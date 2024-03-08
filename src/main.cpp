//------------------------------------------------------------------------------
// File: main.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <libxml/parser.h>

#include "CActionMediator.h"
#include "CArgParser.h"
#include "CCompositeDecider.h"
#include "CDownloading.h"
#include "CDuplicateFileDecider.h"
#include "CLog.h"
#include "CNegationDecider.h"
#include "CRegexDecider.h"

#define INITIAL_RECURSION_DEPTH 0

using namespace std;

/// @brief Worker thread. Takes tasks/actions from queue and performs them.
/// Once CQueueError is thrown during get of next action, finishes.
///
/// @param[in] taskQueue Thread safe action queue to get new tasks.
static void _WorkerThread(CActionQueue& taskQueue);

/// @brief Main.
///
/// @param[in] argc Count of command line arguments.
/// @param[in] argv Command line arguments.
/// @return Status.
/// @retval 0 Success.
/// @retval 2 Error while parsing command line arguments.
int main(int argc, char* argv[])
{
    CArgParser parser;
    parser.Add(CAppParam("--url", true));
    parser.Add(CAppParam("--recursive", false, "false"));
    parser.Add(CAppParam("--leaf-urls-to-local-file", false, "false"));
    parser.Add(CAppParam("--max-recursion-depth", false, "1"));
    parser.Add(CAppParam("--accept-regex"));
    parser.Add(CAppParam("--reject-regex"));
    parser.Add(CAppParam("--download-images", false, "false"));
    parser.Add(CAppParam("--thread-cnt", false, "10"));
    parser.Add(CAppParam("--no-file-overwrite", false, "false"));
    parser.Add(CAppParam("--out-dir", false, "./out"));

    try
    {
        (void)parser.ParseArgs(argc, const_cast<const char**>(argv));

        size_t maxRecursion = 0;
        if ((bool)parser["--recursive"])
        {
            maxRecursion = stoul((string)parser["--max-recursion-depth"]);
        }

        size_t threadCnt = stoul((string)parser["--thread-cnt"]);
        if (threadCnt == 0)
        {
            THROW(invalid_argument, "Invalid argument: Thread count cannot be 0.");
        }

        CCompositeDecider decider;
        if ((bool)parser["--no-file-overwrite"])
        {
            decider.AddDecider(
                make_shared<CDuplicateFileDecider>((string)parser["--out-dir"])
            );
        }

        if (parser["--accept-regex"].m_isSet)
        {
            decider.AddDecider(
                make_shared<CRegexDecider>((string)parser["--accept-regex"])
            );
        }

        if (parser["--reject-regex"].m_isSet)
        {
            decider.AddDecider(
                make_shared<CNegationDecider>(
                    make_shared<CRegexDecider>(
                        (string)parser["--reject-regex"]
                    )
                )
            );
        }

        // Reject all https links.
        decider.AddDecider(
            make_shared<CNegationDecider>(
                make_shared<CRegexDecider>("^https.*")
            )
        );

        CActionQueue taskQueue(threadCnt);
        CActionMediator mediator(taskQueue, maxRecursion,
                                (string)parser["--out-dir"], move(decider),
                                (bool)parser["--leaf-urls-to-local-file"],
                                (bool)parser["--download-images"]);

        {
            shared_ptr<CDownloading> initialTask =
                make_shared<CDownloading>();
            initialTask->AssignMediator(&mediator);
            initialTask->AssignMetadata({INITIAL_RECURSION_DEPTH,
                                        (string)parser["--url"]});
            taskQueue.EnquqeAction(initialTask);
        }

        xmlInitParser();

        vector<thread> workerThreads;
        for (size_t i = 0; i < threadCnt; i++)
        {
            workerThreads.push_back(thread(_WorkerThread, ref(taskQueue)));
        }

        for (auto& thrd : workerThreads)
        {
            thrd.join();
        }

        xmlCleanupParser();
    }
    catch (invalid_argument& e)
    {
        logerror(e.what());
        return 2;
    }

    return 0;
}

static void _WorkerThread(CActionQueue& taskQueue)
{
    while (true)
    {
        SIAction_t pAction;
        try
        {
            pAction = taskQueue.WaitForAction();
        }
        catch(const CQueueError& e)
        {
            break;
        }

        pAction->PerformAction();
    }

    xmlResetLastError();
}
