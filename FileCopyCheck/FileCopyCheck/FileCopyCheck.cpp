// FileCopyCheck.cpp: определяет точку входа для консольного приложения.
//
//#define _CRT_SECURE_NO_WARNINGS
//#define INITGUID
//#define DBINITCONSTANTS
//#ifndef XMLCheckResult	\
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }	\
#endif

//#import "msado15.dll" 
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
//#include "TinyXML\tinyxml2.h"
//#include "TinyXML\tinyxml2.cpp"
#include <objbase.h>
#include <MsXml6.h> raw_interfaces_only

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



void connectSQL()
{
	HRESULT hr = S_OK;
	try
	{
		CoInitialize(NULL);
		// Define string variables. "Provider=SQLOLEDB.1;Persist Security Info=True; User ID=sa;Password=1;Initial Catalog=web-adku;Data Source=srv-tm-ias;"
		_bstr_t strCnn("Provider=SQLOLEDB.1;Persist Security Info=True; User ID=sa;Password=1;Initial Catalog=web-adku;Data Source=srv-tm-ias;");

		_RecordsetPtr pRstAuthors = NULL;

		// Call Create instance to instantiate the Record set
		hr = pRstAuthors.CreateInstance(__uuidof(Recordset));

		if (FAILED(hr))
		{
			printf("Failed creating record set instance\n");
		}

		//Open the Record set for getting records from Author table
		pRstAuthors->Open("SELECT connectionstring from branches", strCnn, adOpenStatic, adLockReadOnly, adCmdText);

		//Declare a variable of type _bstr_t
		_bstr_t valField1;
		int valField2;

		pRstAuthors->MoveFirst();

		//Loop through the Record set
		if (!pRstAuthors->adoEOF)
		{
			while (!pRstAuthors->adoEOF)
			{
				valField1 = pRstAuthors->Fields->GetItem("connectionstring")->Value;
				//valField2 = pRstAuthors->Fields->GetItem("Author_ID")->Value.intVal;
				cout << ((LPCSTR)valField1) << endl;
				pRstAuthors->MoveNext();
			}
		}

	}
	catch (_com_error &ce)
	{
		cout << ce.Description() << endl;
		//printf("Error:%s\n", ce.Description());
		//printf(&ce.Error);
	}

	CoUninitialize();
}

/*bool Test()
{
	tinyxml2::XMLDocument xml_doc;

	tinyxml2::XMLError eResult = xml_doc.LoadFile("C:\\inetpub\\wwwroot\\adku.web\\Web.config");
	
	if (eResult != tinyxml2::XML_SUCCESS) return false; // Здесь возникает ошибка чтения
	tinyxml2::XMLNode* root = xml_doc.FirstChildElement("configuration");
	if (root == nullptr) return false;
	tinyxml2::XMLElement* element = root->FirstChildElement("appSettings")->FirstChildElement("add");
	if (element == nullptr) return false; 
	return true;
}*/


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

	CHK_HR(CreateAndInitDOM(&pXMLDom));

	CHK_HR(VariantFromString(L"C:\\inetpub\\wwwroot\\adku.web\\Web.config", varFileName));
	CHK_HR(pXMLDom->load(varFileName, &varStatus));
	if (varStatus != VARIANT_TRUE)
	{
		CHK_HR(ReportParseError(pXMLDom, "Не получилось загрузить информацию с исхнодного файла"));
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

wstring get_value(wstring source, wstring item) {
	int start_it = source.find(item) + item.length() + 1;
	int end_it = source.find_first_of(';' , start_it);
	if (end_it == -1) {
		end_it = source.find_last_of('"');
	}
	return source.substr(start_it, end_it - start_it);
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
	
	cout << get_value(query_res, L"user id") << endl;
	cout << get_value(query_res, L"data source") << endl;
	cout << get_value(query_res, L"persist security info") << endl;
	cout << get_value(query_res, L"initial catalog") << endl;
	cout << get_value(query_res, L"password") << endl;

	connectSQL();
	//tinyxml2::XMLDocument xml_doc;

	//tinyxml2::XMLError eResult = xml_doc.LoadFile("C:\\inetpub\\wwwroot\\adku.web\\Web.config");

	//if (eResult != tinyxml2::XML_SUCCESS) return false; // Здесь возникает ошибка чтения
	//tinyxml2::XMLNode* root = xml_doc.FirstChildElement("configuration");	
	//tinyxml2::XMLAttribute* attribute= element -> QueryAttribute("key");

	//const char str;
	//eResult = element->Attribute("value", str) //("value", str);
	//string str = element.FindAttribute("value");
	//string str = xml_doc.FirstChildElement("configuration")->FirstChildElement("appSettings")->FirstChildElement("add")->NextSiblingElement()->Attribute("value");
	//std::cout << str << endl;
	

	

	std::system("pause");
    return 0;
} 