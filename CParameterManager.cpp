#include "CParameterManager.h"
#include "common/configure.h"


void CParameterManager::updateParameter(int _index, int _type, DWORD_PTR p)
{
	switch (_type)
	{
	case INSP_HOUGH_LINEFIT:
	{
		PST_LINEFIT_PARAMETER pParam = (PST_LINEFIT_PARAMETER)p;

		m_paramLinefit[_index]._dirxy = pParam->_dirxy;
		m_paramLinefit[_index]._dir = pParam->_dir;
		m_paramLinefit[_index]._slope = pParam->_slope;

		break;
	}
	case FILTER_BINARY:
	{
		PST_BINARY_PARAMETER pParam = (PST_BINARY_PARAMETER)p;

		m_paramBinary[_index]._gray = pParam->_gray;
		m_paramBinary[_index]._mode = pParam->_mode;

		break;
	}
	case INSP_BLOB:
	{
		PST_BLOB_PARAMETER pParam = (PST_BLOB_PARAMETER)p;

		m_paramBlob[_index]._gray = pParam->_gray;
		m_paramBlob[_index]._mode = pParam->_mode;

		break;
	}
	}
}

void CParameterManager::getParameter(int _index, int _type, DWORD_PTR p)
{
	switch (_type)
	{
	case INSP_HOUGH_LINEFIT:
	{
		PST_LINEFIT_PARAMETER pParam = (PST_LINEFIT_PARAMETER)p;

		pParam->_dirxy = m_paramLinefit[_index]._dirxy;
		pParam->_dir = m_paramLinefit[_index]._dir;
		pParam->_slope = m_paramLinefit[_index]._slope;

		break;
	}
	case FILTER_BINARY:
	{
		PST_BINARY_PARAMETER pParam = (PST_BINARY_PARAMETER)p;

		pParam->_gray = m_paramBinary[_index]._gray;
		pParam->_mode = m_paramBinary[_index]._mode;

		break;
	}
	case INSP_BLOB:
	{
		PST_BLOB_PARAMETER pParam = (PST_BLOB_PARAMETER)p;

		pParam->_gray = m_paramBlob[_index]._gray;
		pParam->_mode = m_paramBlob[_index]._mode;

		break;
	}
	}
}