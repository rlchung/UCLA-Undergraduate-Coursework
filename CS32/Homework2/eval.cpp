#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

int precedence(char& operator1);//if paramter 1 has precedence over-or-equal-to operator 2, return true; else, return false
int toInt(char& num);//converts num to an integer value
bool is_operand(char& ch1);//returns true if ch1 is operand; false, otherwise
bool syn_valid(string& statement);//if statement if syntactically valid, then return true, else return false 

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	int ret_value;

	if (syn_valid(infix))
		ret_value = 0;
	else
		return 1;

	//*****************************************
	//INFIX TO POSTFIX CONVERSION**************
	//*****************************************

	stack<char>s_operator;

	postfix = "";

	int incount = infix.size();

	for (int x = 0; x < incount; x++)
	{
		char ch = infix[x];

		switch (ch)
		{
		case ' ':
			break;
		case '0':
			postfix += ch;
			break;
		case '1':
			postfix += ch;
			break;
		case '2':
			postfix += ch;
			break;
		case '3':
			postfix += ch;
			break;
		case '4':
			postfix += ch;
			break;
		case '5':
			postfix += ch;
			break;
		case '6':
			postfix += ch;
			break;
		case '7':
			postfix += ch;
			break;
		case '8':
			postfix += ch;
			break;
		case '9':
			postfix += ch;
			break;
		case '(':
			s_operator.push(ch);
			break;
		case ')':
		{
					while (s_operator.top() != '(')
					{
						postfix += s_operator.top();
						s_operator.pop();
					}
					s_operator.pop();
					break;
		}
		case '!':
		{
					while (!s_operator.empty() && s_operator.top() != '(' && precedence(ch) <= precedence(s_operator.top()))
					{
						postfix += s_operator.top();
						s_operator.pop();
					}
					s_operator.push(ch);
					break;
		}

		case '&':
		{
					while (!s_operator.empty() && s_operator.top() != '(' && precedence(ch) <= precedence(s_operator.top()))
					{
						postfix += s_operator.top();
						s_operator.pop();
					}
					s_operator.push(ch);
					break;
		}

		case '|':
		{
					while (!s_operator.empty() && s_operator.top() != '(' && precedence(ch) <= precedence(s_operator.top()))
					{
						postfix += s_operator.top();
						s_operator.pop();
					}
					s_operator.push(ch);
					break;
		}

		default: //If character is not listed above, return false
			return 1;
		}
	}

	while (!s_operator.empty())
	{
		postfix += s_operator.top();
		s_operator.pop();
	}

	//EVALUATING THE POSTFIX EXPRESSION
	stack<bool>operandBool;

	int postcount = postfix.size();

	for (int x = 0; x < postcount; x++)
	{
		char ch = postfix[x];
		if (is_operand(ch))//if ch is an operand/number
			operandBool.push(values[toInt(ch)]);//Get the bool value that the int in the operand stack represents from values and push onto stack
		else
		{
			bool operand2 = operandBool.top();
			operandBool.pop();
			
			if (ch == '!')
			{
				operandBool.push(!operand2);
			}

			else
			{
				bool operand1 = operandBool.top();
				operandBool.pop();

				if (ch == '&')
					operandBool.push(operand1 && operand2);
				else if (ch == '|')
					operandBool.push(operand1 || operand2);
				else
				{}
			}

		}
	}

	result = operandBool.top();

	return ret_value;
}

int precedence(char& operator1)
{
	if (operator1 == '!')
		return 3;
	if (operator1 == '&')
		return 2;
	if (operator1 == '|')
		return 1;
	else
		return 0;
}

int toInt(char& num)
{
	switch (num)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return -1;
	}
}

bool is_operand(char& ch1)
{
	if (ch1 != '!' && ch1 != '&' && ch1 != '|' && ch1 != ')' && ch1 != '(')
		return true;
	else
		return false;
}

bool syn_valid(string& statement)
{
	//GIVEN EMPTY STATEMENT
	if (statement.size() == 0)
		return false;

	//REDUCES THE EMPTY SPACES IN STRING
	string n_statement = "";

	int state_count = statement.size();

	for (int x = 0; x < state_count; x++)
	{
		if (statement[x] == ' ')
		{
		}
		else
			n_statement += statement[x];
	}

	//CHECK PARENTHESES
	int paren1 = 0;
	int paren2 = 0;

	int nstate_count = n_statement.size();

	for (int z = 0; z < nstate_count; z++)
	{
		if (n_statement[z] == '(')
			paren1++;
		if (n_statement[z] == ')')
			paren2++;
		if (paren2 == 1 && paren1 == 0)//If the first parenthesis encountered is ')' return false
			return false;
	}

	

	if (paren1 != paren2)//Uneven number of parentheses
		return false;

	char prev;
	char cur;

	for (int y = 0; y < nstate_count - 1; y++)
	{
		prev = n_statement[y];
		cur = n_statement[y+1];

		if (y == 0)//if it is the first element
		{
			if (is_operand(prev))//if the first element is a number
			{
				if (cur == '!' || cur == ')')
					return false; 
			}
		}

		if (is_operand(prev))//if there are two consecutive operands 
		{
			if (is_operand(cur))
			{
				return false;
			}
		}

		if (prev == '(' && cur == ')')//empty parentheses
			return false;

		if (is_operand(prev))//An operand cannot be followed by a '!' or a '('
		{
			if (cur == '!' || cur == '(')
				return false;
		}

		if (prev == '!' || prev == '&' || prev == '|')// '!&' , '!|' , '!)', '&&', '&|', '&)', '|&', '||', '|)' are false
		{
			if (cur == '&' || cur == '|' || cur == ')')
				return false;
		}

		if (cur == '!'|| cur == '&' || cur == '|')//'(!', '(&', '(|' are false 
		{
			if (prev == '(')
				return false;
		}

		if (y + 2 == n_statement.size())//If current is the last element
		{
			if (is_operand(cur))//If the last element is a number
			{
				if (is_operand(prev) || prev == '(')// ## or (# is false
					return false;
			}

			if (!is_operand(cur))//If the last element is not a number
			{
				if (cur != ')')//If the last operator is anything besides a ')' return false
					return false;
			}
		}
	}
	return true;

}