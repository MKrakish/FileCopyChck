// FileCopyCheck.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#include <objbase.h>
#include <MsXml6.h> //raw_interfaces_only

#include "filecopyfunc.h"

#import "c:\Program Files (x86)\Common Files\System\ADO\msado20.tlb"  rename ("EOF","adoEOF") no_namespace




#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)
// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)
// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

// OLE DB - ODBC провайдеры

using namespace std;
using namespace std::tr2::sys;

DWORD NetConnect(wstring computer) {
	DWORD dwRetVal;
	wstring shit = L"\\\\" + computer + L"\\c$";
	NETRESOURCE nr;
	DWORD dwFlags;
	nr.dwType = RESOURCETYPE_DISK;
	nr.lpLocalName = NULL; // explicitly set this to NULL
	nr.lpRemoteName = const_cast<LPWSTR>(shit.c_str());
	nr.lpProvider = NULL;

	dwFlags = CONNECT_UPDATE_PROFILE;

	dwRetVal = WNetAddConnection2(&nr, L"1", L"Администратор", dwFlags);

	return dwRetVal;
}



void connectSQL(wstring id, wstring pass, wstring dsource, wstring ctlg, wstring security)
{
	HRESULT hr = S_OK;
	
	try
	{
		CoInitialize(NULL);
		// Define string variables. "Provider=SQLOLEDB.1;Persist Security Info=True; User ID=sa;Password=1;Initial Catalog=web-adku;Data Source=srv-tm-ias;"
		wstring initialstring = L"Provider=SQLOLEDB.1;Persist Security Info=" + security + L"; User ID=" + id + L";Password=" + pass + L";Initial Catalog=" + ctlg + L";Data Source=" + dsource + L";";
		assert(!initialstring.empty());
		BSTR bstr_initstr = SysAllocStringLen(initialstring.data(), initialstring.size());
		_bstr_t strCnn(bstr_initstr);

		_RecordsetPtr pRstServerNames = NULL;

		// Call Create instance to instantiate the Record set
		hr = pRstServerNames.CreateInstance(__uuidof(Recordset));

		if (FAILED(hr))
		{
			printf("Failed creating record set instance\n");
		}

		//Open the Record set for getting records from Author table
		pRstServerNames->Open("SELECT connectionstring from branches", strCnn, adOpenStatic, adLockReadOnly, adCmdText);

		//Declare a variable of type _bstr_t
		_bstr_t valField1;
		//int valField2;

		pRstServerNames->MoveFirst();
		ofstream log("log.txt", ios_base::app);
		//Loop through the Record set
		if (!pRstServerNames->adoEOF)
		{
			wstring *serverNameArray = new wstring[20]; int  i = 0;
			while (!pRstServerNames->adoEOF)
			{
				valField1 = pRstServerNames->Fields->GetItem("connectionstring")->Value;
				//valField2 = pRstAuthors->Fields->GetItem("Author_ID")->Value.intVal;
				//cout << ((LPCSTR)valField1) << endl;
				wstring serverName(valField1, SysStringLen(valField1));
				//cout << get_value(serverName, L"server")<< endl;
				 int c = 0;
					serverNameArray[i] = get_value(serverName, L"server");
					for (int j = 0; j < i; j++) {
						if (serverNameArray[i] == serverNameArray[j]) {
							c = 1;
						}
					}
					if (c != 1) {
						cout << "\nПодключаюсь к " << serverNameArray[i] << endl;
						log << "\nПодключаюсь к " << serverNameArray[i] << endl;
						if (NetConnect(serverNameArray[i]) != NO_ERROR) {
							cout << "\nОшибка соединения с " << serverNameArray[i] << endl;
							log << "\nОшибка соединения с " << serverNameArray[i] << endl;
							continue;
						}
						cout << "\nКопирую на "<<serverNameArray[i] << endl;
						log << "\nКопирую на " << serverNameArray[i] << endl;
						wstring copyTo= L"\\\\" + serverNameArray[i] + L"\\c$\\Temp\\SUZUN";
						copyFolder(copyTo);
					}
				
				
				pRstServerNames->MoveNext();
				i++;
			}
		}
		log.close();
	}
	catch (_com_error &ce)
	{
		ofstream log("log.txt", ios_base::app);
		cout << ce.Description() << endl;
		log << ce.Description() << endl;
		log.close();
	}
	
	CoUninitialize();
}


// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant)
{
	HRESULT hr = S_OK;
	BSTR bstr = SysAllocString(wszValue);
	CHK_ALLOC(bstr);

	V_VT(&Variant) = VT_BSTR;
	V_BSTR(&Variant) = bstr;

CleanUp:
	return hr;
} 

HRESULT CreateAndInitDOM(IXMLDOMDocument **ppDoc)
{
	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
	if (SUCCEEDED(hr))
	{
		// these methods should not fail so don't inspect result
		(*ppDoc)->put_async(VARIANT_FALSE);
		(*ppDoc)->put_validateOnParse(VARIANT_FALSE);
		(*ppDoc)->put_resolveExternals(VARIANT_FALSE);
	}
	return hr;
}

HRESULT ReportParseError(IXMLDOMDocument *pDoc, char *szDesc)
{
	HRESULT hr = S_OK;
	HRESULT hrRet = E_FAIL; // Default error code if failed to get from parse error.
	IXMLDOMParseError *pXMLErr = NULL;
	BSTR bstrReason = NULL;

	CHK_HR(pDoc->get_parseError(&pXMLErr));
	CHK_HR(pXMLErr->get_errorCode(&hrRet));
	CHK_HR(pXMLErr->get_reason(&bstrReason));
	printf("%s\n%S\n", szDesc, bstrReason);

CleanUp:
	SAFE_RELEASE(pXMLErr);
	SysFreeString(bstrReason);
	return hrRet;
}


wstring queryNodes()
{
	HRESULT hr = S_OK;
	IXMLDOMDocument *pXMLDom = NULL;
	IXMLDOMNodeList *pNodes = NULL;
	IXMLDOMNode *pNode = NULL;

	BSTR bstrQuery1 = NULL;
	BSTR bstrNodeName = NULL;
	BSTR bstrNodeValue = NULL;
	VARIANT_BOOL varStatus;
	VARIANT varFileName;
	VariantInit(&varFileName);
	wstring nullstring = L"";
	CHK_HR(CreateAndInitDOM(&pXMLDom));

	CHK_HR(VariantFromString(L"C:\\inetpub\\wwwroot\\adku.web\\Web.config", varFileName));
	CHK_HR(pXMLDom->load(varFileName, &varStatus));
	if (varStatus != VARIANT_TRUE)
	{
		CHK_HR(ReportParseError(pXMLDom, "Не получилось загрузить информацию с исхнодного файла"));
		return nullstring;
	}

	// Query a single node.
	bstrQuery1 = SysAllocString(L"configuration/appSettings/add[@key = \"ConnectionString\"]/@value");
	CHK_ALLOC(bstrQuery1);
	CHK_HR(pXMLDom->selectSingleNode(bstrQuery1, &pNode));
	if (pNode)
	{

		CHK_HR(pNode->get_nodeName(&bstrNodeName));
		SysFreeString(bstrNodeName);
		CHK_HR(pNode->get_xml(&bstrNodeValue));
		SysFreeString(bstrNodeValue);
		SAFE_RELEASE(pNode);
		assert(bstrNodeValue != nullptr);
		std::wstring str(bstrNodeValue, SysStringLen(bstrNodeValue));
		return str;
	}
	else
	{
		CHK_HR(ReportParseError(pXMLDom, "Ошибка при парсинге файла"));
		return nullstring;
	}

	

CleanUp:
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pNodes);
	SAFE_RELEASE(pNode);
	SysFreeString(bstrQuery1);
	SysFreeString(bstrNodeName);
	SysFreeString(bstrNodeValue);
	VariantClear(&varFileName);

	
}


int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");
	
	wstring query_res;
	
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		query_res =  queryNodes();

		CoUninitialize();
	}
	
	
	wstring id				= get_value(query_res, L"user id");
	wstring dataSource		= get_value(query_res, L"data source");
	wstring securityInfo	= get_value(query_res, L"persist security info");
	wstring catalog			= get_value(query_res, L"initial catalog");
	wstring pass			= get_value(query_res, L"password");
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	std::cout << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	std::cout << "ПКБ \"АСУ-Нефть\" 2016." << endl;
	ofstream log("log.txt", ios_base::trunc);
	log << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	connectSQL(id, pass, dataSource, catalog, securityInfo);
	log.close();
	std::system("pause");
    return 0;
} 