//------------------------------------------------------------------------------
// File: CProcessing.cpp
// Author: Chvojka Vojtech, CTU Prague
//------------------------------------------------------------------------------
#include "CProcessing.h"

using namespace std;

CProcessing::CProcessing(const string& rawData)
    : m_rawData(rawData)
{}

void CProcessing::Run()
{
    m_metadata->m_filename =
        m_mediator->GetUrlTranslator()->GetAbsouluteFilePath(m_metadata->m_url);
    Process();
}

void CProcessing::Process()
{
    m_processedData = move(m_rawData);
}

void CProcessing::OnActionFinish()
{
    m_mediator->OnProcessingFinished(this);
}

void CProcessing::PrintSelf(ostream& os) const
{
    os << "Processing of " << m_metadata->m_url;
}
