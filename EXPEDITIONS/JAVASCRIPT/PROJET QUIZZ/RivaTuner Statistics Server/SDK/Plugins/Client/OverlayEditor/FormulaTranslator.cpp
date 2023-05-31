// ForumaTranslator.cpp: implementation of the CFormulaTranslator class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FormulaTranslator.h"
//////////////////////////////////////////////////////////////////////
#include <math.h>
//////////////////////////////////////////////////////////////////////
#ifndef _RTFCERRORS_H_INCLUDED_
#include "RTFCErrors.h"
#endif
#ifndef _FORMULALEXICALANALYZER_H_INCLUDED_
#include "FormulaLexicalAnalyzer.h"
#endif

#include "FormulaTranslatorVariableProvider.h"

#include <float.h>

#define UNARY_MINUS	'@'
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFormulaTranslator::CFormulaTranslator()
{
	m_pVariableProvider = NULL;

	Init();
}
//////////////////////////////////////////////////////////////////////
CFormulaTranslator::~CFormulaTranslator()
{
	DestroyOperandsStack();
}
//////////////////////////////////////////////////////////////////////
HRESULT CFormulaTranslator::Translate(LPCSTR lpFormula)
{
	DestroyOperandsStack();
		//destroy operands stack, we'll use it as operation stack during translation

	DWORD dwIndex	= 0;
		//init lexeme sort index

	HRESULT errCode = Analyze(lpFormula);
		//do lexical analysis

	if (FAILED(errCode))
		return errCode;
			//return error if lexical analysis failed

	LPLEXEME_DESC pLexeme		= GetHeadLexeme();
	LPLEXEME_DESC pLexemePrev	= NULL;

	while (pLexeme)
		//process lexemes list and replace unary minus lexemes
	{
		if (IsOperationLexeme(pLexeme))
		{
			if (pLexeme->dwData == '-')
			{
				if (!pLexemePrev || (IsOperationLexeme(pLexemePrev) && !IsOperationCloseBracketLexeme(pLexemePrev)))
					pLexeme->dwData = UNARY_MINUS;
			}
		}

		pLexemePrev	= pLexeme;
		pLexeme		= GetNextLexeme();
	}

	pLexeme = GetHeadLexeme();


	while (pLexeme)
		//process lexemes list
	{
		if (IsOperationLexeme(pLexeme))
			//if it is operation lexeme, we'll push it in operations stack
		{
			DWORD dwPriority = InitLexemePriority(pLexeme);
				//try to init operation priority 

			if (dwPriority != 0xffffffff)
				//if operation priority is initialized properly, push it
			{
				if (m_stack.IsEmpty())
					//if the stack is empty, simply push the operation
					m_stack.Push(pLexeme);
				else
					//otherwise analyze operation type and process brackets
				{
					if (IsOperationOpenBracketLexeme(pLexeme))
						m_stack.Push(pLexeme);
						//if it is open bracket, simply push it
					else
						if (IsOperationCloseBracketLexeme(pLexeme))
							//if it is close bracket, pop all operations until
							//close bracket is poped
						{

							LPLEXEME_DESC pLexemeFromStack = NULL;

							while (!m_stack.IsEmpty())
							{
								pLexemeFromStack = (LPLEXEME_DESC)m_stack.Pop();
								
								if (IsOperationOpenBracketLexeme(pLexemeFromStack))
									break;

								pLexemeFromStack->dwReserved = dwIndex++;
							}

							if (!pLexemeFromStack || !IsOperationOpenBracketLexeme(pLexemeFromStack))
								//if we haven't found open bracket, then formula is invalid
							{
								m_stack.RemoveAll();
									//don't forget to clean up operations stack before exit

								return FORMULATRANSLATOR_E_UNEXPECTED_BRACKET;
								//if we have not met close bracket, then formula is invalid
							}
						}
						else
							//otherwise we'll pop all the operations from the stack
							//until operation with less or equal priority is at stack
						{

							LPLEXEME_DESC pLexemeFromStack = (LPLEXEME_DESC)m_stack.Peek();	

							if (pLexemeFromStack->dwReserved < dwPriority)
								m_stack.Push(pLexeme);
							else
							{
								while (pLexemeFromStack->dwReserved > dwPriority)
								{
									pLexemeFromStack				= (LPLEXEME_DESC)m_stack.Pop();
									pLexemeFromStack->dwReserved	= dwIndex++;

									pLexemeFromStack				= (LPLEXEME_DESC)m_stack.Peek();	

									if (!pLexemeFromStack)
										break;
								}

								m_stack.Push(pLexeme);
							}
						}
				}
			}
			else
			{
				m_stack.RemoveAll();
					//don't forget to clean up operations stack before exit

				return FORMULATRANSLATOR_E_UNKNOWN_OPERATION;
			}
		}
		else
			//otherwise it is operand, and we simply set its' lexeme sort index
			pLexeme->dwReserved = dwIndex++; 

		pLexeme = GetNextLexeme();
	}

	while (!m_stack.IsEmpty())
		//pop operations from the stack and return error if there are any bracket
		//operations left
	{
		LPLEXEME_DESC pLexemeFromStack	= (LPLEXEME_DESC)m_stack.Pop();
		pLexemeFromStack->dwReserved	= dwIndex++;	

		if (IsOperationBracketLexeme(pLexemeFromStack))
		{
			m_stack.RemoveAll();
				//don't forget to clean up operations stack before exit

			return FORMULATRANSLATOR_E_BRACKET_EXPECTED;
		}
	}

	m_stack.RemoveAll();
		//clean up operations stack

	//loop through the list of lexemes and remove all bracket operation lexemes

	pLexeme = GetHeadLexeme();

	while (pLexeme)
		if (IsOperationBracketLexeme(pLexeme))
			pLexeme = DestroyThisLexeme();
		else
			pLexeme = GetNextLexeme();

	m_lexemesList.Sort(TRUE, SortLexemesHelper);
		//sort lexemes list using lexeme indices we set

	return S_OK;
}
//////////////////////////////////////////////////////////////////////
DWORD CFormulaTranslator::InitLexemePriority(LPLEXEME_DESC lpLexeme)
{
	DWORD dwPriority	= 0;

	if (lpLexeme->dwData == '(')
	{
		lpLexeme->dwReserved = dwPriority;
		return dwPriority;
	}
	dwPriority++;

	if (lpLexeme->dwData == ')')
	{
		lpLexeme->dwReserved = dwPriority;
		return dwPriority;
	}
	dwPriority++;

	if ((lpLexeme->dwData == '+') || (lpLexeme->dwData == '-'))
	{
		lpLexeme->dwReserved = dwPriority;
		return dwPriority;
	}
	dwPriority++;

	if ((lpLexeme->dwData == '*') || (lpLexeme->dwData == '/') || (lpLexeme->dwData == '%'))
	{
		lpLexeme->dwReserved = dwPriority;
		return dwPriority;
	}
	dwPriority++;

	if (lpLexeme->dwData == '^')
	{
		lpLexeme->dwReserved = dwPriority;
		return dwPriority;
	}

	if (lpLexeme->dwData == UNARY_MINUS)
	{
		lpLexeme->dwReserved = dwPriority;
		return dwPriority;
	}
	dwPriority++;

	return 0xffffffff;
}
//////////////////////////////////////////////////////////////////////
int CFormulaTranslator::SortLexemesHelper(LPPTR_LIST_NODE pLexemeNode0, LPPTR_LIST_NODE pLexemeNode1)
{
	LPLEXEME_DESC pLexeme0 = (LPLEXEME_DESC)pLexemeNode0->pData;
	LPLEXEME_DESC pLexeme1 = (LPLEXEME_DESC)pLexemeNode1->pData;

	if (pLexeme0->dwReserved > pLexeme1->dwReserved)
		return 1;

	if (pLexeme0->dwReserved < pLexeme1->dwReserved)
		return -1;

	return 0;
}
//////////////////////////////////////////////////////////////////////
CStringLite CFormulaTranslator::Format()
{
	CStringLite strResult;

	LPLEXEME_DESC pLexeme = GetHeadLexeme();

	while (pLexeme)
	{
		CStringLite strLexeme;

		if (IsOperationLexeme(pLexeme))
		{
			switch (pLexeme->dwData)
			{
			case UNARY_MINUS:
				strLexeme = "-";
				break;
			default:
				strLexeme.Format("%c", pLexeme->dwData);
				break;
			}
		}

		if (IsConstIntLexeme(pLexeme))
			strLexeme.Format("%d", pLexeme->dwData);

		if (IsConstFltLexeme(pLexeme))
			strLexeme.Format("%f", LONG_AS_FLOAT(pLexeme->dwData));

		if (IsVarLexeme(pLexeme))
			strLexeme.Format("%s", pLexeme->lpData);

		if (!strResult.IsEmpty())
			strResult += " ";

		strResult += strLexeme;

		pLexeme = GetNextLexeme();

	}

	return strResult;
}
//////////////////////////////////////////////////////////////////////
HRESULT CFormulaTranslator::Calc()
{
	DestroyOperandsStack();

	LPLEXEME_DESC pLexeme = GetHeadLexeme();

	while (pLexeme)
	{
		if (IsConstIntLexeme(pLexeme))
			PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, pLexeme->dwData);

		if (IsConstFltLexeme(pLexeme))
			PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, pLexeme->dwData);

		if (IsVarLexeme(pLexeme))
		{
			if (!PushVar((LPSTR)pLexeme->lpData))
				return FORMULATRANSLATOR_E_UNDECLARED_VARIABLE;
		}

		if (IsOperationLexeme(pLexeme))
		{
			if (pLexeme->dwData == UNARY_MINUS)
			{
				LPFORMULATRANSLATOR_VALUE_DESC pOperand = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Pop();

				if (!pOperand)
					return FORMULATRANSLATOR_E_OPERAND_EXPECTED;

				CalcUnaryMinus(pOperand);

				if (pOperand)
					delete pOperand;
			}
			else
			{
				LPFORMULATRANSLATOR_VALUE_DESC pOperand1 = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Pop();
				LPFORMULATRANSLATOR_VALUE_DESC pOperand0 = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Pop();

				if (!pOperand0 || !pOperand1)
					return FORMULATRANSLATOR_E_OPERAND_EXPECTED;

				switch (pLexeme->dwData)
				{
				case '+':
					CalcAdd(pOperand0, pOperand1);
					break;
				case '-':
					CalcSub(pOperand0, pOperand1);
					break;
				case '*':
					CalcMul(pOperand0, pOperand1);
					break;
				case '/':
					CalcDiv(pOperand0, pOperand1);
					break;
				case '%':
					CalcRem(pOperand0, pOperand1);
					break;
				case '^':
					CalcPow(pOperand0, pOperand1);
					break;
				}

				if (pOperand0)
					delete pOperand0;

				if (pOperand1)
					delete pOperand1;
			}
		}

		pLexeme = GetNextLexeme();
	}

	if (m_stack.GetCount() > 1)
		return FORMULATRANSLATOR_E_OPERATION_EXPECTED;

	return S_OK;
}
//////////////////////////////////////////////////////////////////////
void CFormulaTranslator::PushVal(DWORD dwType, LONG dwData)
{
	LPFORMULATRANSLATOR_VALUE_DESC pDesc = new FORMULATRANSLATOR_VALUE_DESC;

	pDesc->dwType = dwType;
	pDesc->dwData = dwData;

	m_stack.Push(pDesc);
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::PushVar(LPCSTR lpName)
{
	if (!m_pVariableProvider)
		return FALSE;

	LPFORMULATRANSLATOR_VALUE_DESC pDesc = new FORMULATRANSLATOR_VALUE_DESC;

	if (!m_pVariableProvider->GetVariable(lpName, pDesc))
	{
		delete pDesc;
		return FALSE;
	}

	m_stack.Push(pDesc);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::CalcUnaryMinus(LPFORMULATRANSLATOR_VALUE_DESC pOperand)
{
	if (IsIntValue(pOperand))
	{
		LONG val	= -GetIntValue(pOperand);

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, val);
	}
	else
	{
		FLOAT val	= (GetFltValue(pOperand) == FLT_MAX) ? FLT_MAX : -GetFltValue(pOperand);

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLOAT_AS_LONG(val));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::CalcAdd(LPFORMULATRANSLATOR_VALUE_DESC pOperand0, LPFORMULATRANSLATOR_VALUE_DESC pOperand1)
{
	if (IsIntValue(pOperand0) && IsIntValue(pOperand1))
	{
		LONG val0	= GetIntValue(pOperand0);
		LONG val1	= GetIntValue(pOperand1);
		LONG val	= val0 + val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, val);
	}
	else
	{
		FLOAT val0	= GetFltValue(pOperand0);
		FLOAT val1	= GetFltValue(pOperand1);
		FLOAT val	= ((val0 == FLT_MAX) || (val1 == FLT_MAX)) ? FLT_MAX : val0 + val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLOAT_AS_LONG(val));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::CalcSub(LPFORMULATRANSLATOR_VALUE_DESC pOperand0, LPFORMULATRANSLATOR_VALUE_DESC pOperand1)
{
	if (IsIntValue(pOperand0) && IsIntValue(pOperand1))
	{
		LONG val0	= GetIntValue(pOperand0);
		LONG val1	= GetIntValue(pOperand1);
		LONG val	= val0 - val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, val);
	}
	else
	{
		FLOAT val0	= GetFltValue(pOperand0);
		FLOAT val1	= GetFltValue(pOperand1);
		FLOAT val	= ((val0 == FLT_MAX) || (val1 == FLT_MAX)) ? FLT_MAX : val0 - val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLOAT_AS_LONG(val));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::CalcMul(LPFORMULATRANSLATOR_VALUE_DESC pOperand0, LPFORMULATRANSLATOR_VALUE_DESC pOperand1)
{
	if (IsIntValue(pOperand0) && IsIntValue(pOperand1))
	{
		LONG val0	= GetIntValue(pOperand0);
		LONG val1	= GetIntValue(pOperand1);
		LONG val	= val0 * val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, val);
	}
	else
	{
		FLOAT val0	= GetFltValue(pOperand0);
		FLOAT val1	= GetFltValue(pOperand1);
		FLOAT val	= ((val0 == FLT_MAX) || (val1 == FLT_MAX)) ? FLT_MAX : val0 * val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLOAT_AS_LONG(val));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::CalcDiv(LPFORMULATRANSLATOR_VALUE_DESC pOperand0, LPFORMULATRANSLATOR_VALUE_DESC pOperand1)
{
	if (IsIntValue(pOperand0) && IsIntValue(pOperand1))
	{
		LONG val0	= GetIntValue(pOperand0);
		LONG val1	= GetIntValue(pOperand1);

		if (val1 == 0)
			PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLT_MAX_AS_LONG);
		else
		{
			LONG val	= val0 / val1;

			PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, val);
		}
	}
	else
	{
		FLOAT val0	= GetFltValue(pOperand0);
		FLOAT val1	= GetFltValue(pOperand1);
		FLOAT val	= ((val0 == FLT_MAX) || (val1 == FLT_MAX) || (val1 == 0)) ? FLT_MAX : val0 / val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLOAT_AS_LONG(val));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::CalcRem(LPFORMULATRANSLATOR_VALUE_DESC pOperand0, LPFORMULATRANSLATOR_VALUE_DESC pOperand1)
{
	if (IsIntValue(pOperand0) && IsIntValue(pOperand1))
	{
		LONG val0	= GetIntValue(pOperand0);
		LONG val1	= GetIntValue(pOperand1);

		if (val1 == 0)
			PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLT_MAX_AS_LONG);
		else
		{
			LONG val	= val0 % val1;

			PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, val);
		}
	}
	else
	{
		FLOAT val0	= GetFltValue(pOperand0);
		FLOAT val1	= GetFltValue(pOperand1);
		FLOAT val	= ((val0 == FLT_MAX) || (val1 == FLT_MAX) || (val1 == 0)) ? FLT_MAX : (LONG)val0 % (LONG)val1;

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLOAT_AS_LONG(val));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::CalcPow(LPFORMULATRANSLATOR_VALUE_DESC pOperand0, LPFORMULATRANSLATOR_VALUE_DESC pOperand1)
{
	if (IsIntValue(pOperand0) && IsIntValue(pOperand1))
	{
		LONG val0	= GetIntValue(pOperand0);
		LONG val1	= GetIntValue(pOperand1);
		LONG val	= (LONG)pow((float)val0, (float)val1);

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_INT, val);
	}
	else
	{
		FLOAT val0	= GetFltValue(pOperand0);
		FLOAT val1	= GetFltValue(pOperand1);
		FLOAT val	= ((val0 == FLT_MAX) || (val1 == FLT_MAX)) ? FLT_MAX : (FLOAT)pow(val0, val1);

		PushVal(FORMULATRANSLATOR_VALUE_TYPE_FLT, FLOAT_AS_LONG(val));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::IsIntValue(LPFORMULATRANSLATOR_VALUE_DESC pDesc)
{
	if (pDesc)
		return pDesc->dwType == FORMULATRANSLATOR_VALUE_TYPE_INT;

	return FALSE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::IsFltValue(LPFORMULATRANSLATOR_VALUE_DESC pDesc)
{
	if (pDesc)
		return pDesc->dwType == FORMULATRANSLATOR_VALUE_TYPE_FLT;

	return FALSE;
}
//////////////////////////////////////////////////////////////////////
LONG CFormulaTranslator::GetIntValue(LPFORMULATRANSLATOR_VALUE_DESC pDesc)
{
	if (IsIntValue(pDesc))
		return pDesc->dwData;

	if (IsFltValue(pDesc))
		return (LONG)(*(FLOAT*)&pDesc->dwData);

	return 0;
}
//////////////////////////////////////////////////////////////////////
FLOAT CFormulaTranslator::GetFltValue(LPFORMULATRANSLATOR_VALUE_DESC pDesc)
{
	if (IsIntValue(pDesc))
		return (FLOAT)pDesc->dwData;

	if (IsFltValue(pDesc))
		return *(FLOAT*)&pDesc->dwData;

	return 0;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::IsIntValue()
{
	LPFORMULATRANSLATOR_VALUE_DESC pDesc = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Peek();

	if (pDesc)
		return IsIntValue(pDesc);

	return FALSE;
}
//////////////////////////////////////////////////////////////////////
BOOL CFormulaTranslator::IsFltValue()
{
	LPFORMULATRANSLATOR_VALUE_DESC pDesc = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Peek();

	if (pDesc)
		return IsFltValue(pDesc);

	return FALSE;
}
//////////////////////////////////////////////////////////////////////
LONG CFormulaTranslator::GetIntValue()
{
	LPFORMULATRANSLATOR_VALUE_DESC pDesc = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Peek();

	if (pDesc)
		return GetIntValue(pDesc);

	return 0;
}
//////////////////////////////////////////////////////////////////////
FLOAT CFormulaTranslator::GetFltValue()
{
	LPFORMULATRANSLATOR_VALUE_DESC pDesc = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Peek();

	if (pDesc)
		return GetFltValue(pDesc);

	return 0;
}
//////////////////////////////////////////////////////////////////////
void CFormulaTranslator::DestroyOperandsStack()
{
	while (!m_stack.IsEmpty())
	{
		LPFORMULATRANSLATOR_VALUE_DESC pDesc = (LPFORMULATRANSLATOR_VALUE_DESC)m_stack.Pop();

		if (pDesc)
			delete pDesc;
	}

	m_stack.RemoveAll();
}
//////////////////////////////////////////////////////////////////////
void CFormulaTranslator::SetVariableProvider(CFormulaTranslatorVariableProvider *pVariableProvider)
{
	m_pVariableProvider = pVariableProvider;
}
//////////////////////////////////////////////////////////////////////
