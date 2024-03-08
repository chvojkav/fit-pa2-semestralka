//------------------------------------------------------------------------------
// File: CSaving.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include <fstream>
#include <filesystem>

#include "CLog.h"


#include "CSaving.h"
using namespace std;
using namespace std::filesystem;


CSaving::CSaving(string&& data)
    : m_data(data)
{}

void CSaving::Run()
{
    path filename = m_metadata->m_filename;
    create_directories(filename.parent_path());

    ofstream outFile(filename, ios::binary);
    
    if (!outFile.is_open())
    {
        THROW(runtime_error, "Could not open file: " << m_metadata->m_filename);
    }

    outFile.write(m_data.data(), m_data.size());

    if (!outFile)
    {
        THROW(runtime_error, "Error while saving file: "
                             << m_metadata->m_filename);
    }
}

void CSaving::OnActionFinish()
{
    m_mediator->OnSavingFinished(this);
}

void CSaving::PrintSelf(ostream& os) const
{
    os << "Saving of " << m_metadata->m_filename;
}
