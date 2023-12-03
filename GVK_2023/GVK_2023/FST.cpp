#include "FST.h"
#include "LT.h"
#include <vector>
#include "Error.h"

namespace FST {
	RELATION::RELATION(char c, short ns) {
		symbol = c;
		nnode = ns;
	}

	NODE::NODE() {
		n_relation = 0;
		RELATION* relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...) {
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];
		for (int i = 0; i < n; i++) {
			relations[i] = p[i];
		}
	}

	FST::FST(std::vector<char> s, short ns, NODE n, ...) {
		string = s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE* p = &n;
		for (int i = 0; i < ns; i++) {
			nodes[i] = p[i];
		};
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1;
	};

	bool step(FST& fst, short*& rstates) {
		bool rc = false;
		std::swap(rstates, fst.rstates);
		for (int i = 0; i < fst.nstates; i++) {
			if (rstates[i] == fst.position) {
				for (int j = 0; j < fst.nodes[i].n_relation; j++) {
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) {
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
						return rc;
					}
				}
			}
		}
		return rc;
	}

	bool execute(FST& fst) {
		fst.FSTreturn();
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short lstring = fst.string.size();
		bool rc = true;
		for (int i = 0; i < lstring && rc; i++) {
			fst.position++;
			rc = step(fst, rstates);
		};
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}

	bool isSymbolIsStopSymbol(char ch) {
		char stopSymbols[] = {
			' ', ';', ':', ',', '(', '{', '}',')', '=', '!', '<', '>', '\'', '\"', '\n'
		};
		int numberOfStopSymbols = 15;
		for (int i = 0; i < numberOfStopSymbols; i++) {
			if (ch == stopSymbols[i]) {
				return true;
			}
		}
		return false;
	}

	void show(std::vector<char>buff) {
		for (int i = 0; i < buff.size(); i++) {
			std::cout << buff[i];
		}
		std::cout << std::endl;
	}

	void Analyze(In::IN in, LT::LexTable& lextable, IT::IdTable& idtable) {
		std::vector<char>str;

		FST lex_hallow(
			str,
			7,
			NODE(1, RELATION('h', 1)),
			NODE(1, RELATION('a', 2)),
			NODE(1, RELATION('l', 3)),
			NODE(1, RELATION('l', 4)),
			NODE(1, RELATION('o', 5)),
			NODE(1, RELATION('w', 6)),
			NODE()
		);

		FST lex_byte(
			str,
			5,
			NODE(1, RELATION('b', 1)),
			NODE(1, RELATION('y', 2)),
			NODE(1, RELATION('t', 3)),
			NODE(1, RELATION('e', 4)),
			NODE()
		);

		FST lex_text(
			str,
			5,
			NODE(1, RELATION('t', 1)),
			NODE(1, RELATION('e', 2)),
			NODE(1, RELATION('x', 3)),
			NODE(1, RELATION('t', 4)),
			NODE()
		);

		FST lex_boolean(
			str,
			8,
			NODE(1, RELATION('b', 1)),
			NODE(1, RELATION('o', 2)),
			NODE(1, RELATION('o', 3)),
			NODE(1, RELATION('l', 4)),
			NODE(1, RELATION('e', 5)),
			NODE(1, RELATION('a', 6)),
			NODE(1, RELATION('n', 7)),
			NODE()
		);

		FST lex_symbol(
			str,
			7,
			NODE(1, RELATION('s', 1)),
			NODE(1, RELATION('y', 2)),
			NODE(1, RELATION('m', 3)),
			NODE(1, RELATION('b', 4)),
			NODE(1, RELATION('o', 5)),
			NODE(1, RELATION('l', 6)),
			NODE()
		);

		FST lex_declare(
			str,
			8,
			NODE(1, RELATION('d', 1)),
			NODE(1, RELATION('e', 2)),
			NODE(1, RELATION('c', 3)),
			NODE(1, RELATION('l', 4)),
			NODE(1, RELATION('a', 5)),
			NODE(1, RELATION('r', 6)),
			NODE(1, RELATION('e', 7)),
			NODE()
		);

		FST lex_return(
			str,
			7,
			NODE(1, RELATION('r', 1)),
			NODE(1, RELATION('e', 2)),
			NODE(1, RELATION('t', 3)),
			NODE(1, RELATION('u', 4)),
			NODE(1, RELATION('r', 5)),
			NODE(1, RELATION('n', 6)),
			NODE()
		);

		FST lex_display(
			str,
			8,
			NODE(1, RELATION('d', 1)),
			NODE(1, RELATION('i', 2)),
			NODE(1, RELATION('s', 3)),
			NODE(1, RELATION('p', 4)),
			NODE(1, RELATION('l', 5)),
			NODE(1, RELATION('a', 6)),
			NODE(1, RELATION('y', 7)),
			NODE()
		);

		FST lex_main(
			str,
			5,
			NODE(1, RELATION('m', 1)),
			NODE(1, RELATION('a', 2)),
			NODE(1, RELATION('i', 3)),
			NODE(1, RELATION('n', 4)),
			NODE()
		);

		FST lex_assigment(
			str,
			2,
			NODE(1, RELATION(':', 1)),
			NODE()
		);

		FST lex_semicolon(
			str,
			2,
			NODE(1, RELATION(';', 1)),
			NODE()
		);

		FST lex_comma(
			str,
			2,
			NODE(1, RELATION(',', 1)),
			NODE()
		);

		FST lex_leftbrace(
			str,
			2,
			NODE(1, RELATION('{', 1)),
			NODE()
		);

		FST lex_bracelet(
			str,
			2,
			NODE(1, RELATION('}', 1)),
			NODE()
		);

		FST lex_leftthesis(
			str,
			2,
			NODE(1, RELATION('(', 1)),
			NODE()
		);

		FST lex_rightthesis(
			str,
			2,
			NODE(1, RELATION(')', 1)),
			NODE()
		);
		// математические операции
		FST lex_plus(
			str,
			2,
			NODE(1, RELATION('+', 1)),
			NODE()
		);
		FST lex_minus(
			str,
			2,
			NODE(1, RELATION('-', 1)),
			NODE()
		);
		FST lex_start(
			str,
			2,
			NODE(1, RELATION('*', 1)),
			NODE()
		);
		FST lex_dirslash(
			str,
			2,
			NODE(1, RELATION('/', 1)),
			NODE()
		);

		// логические операторы
		FST lex_more(
			str,
			2,
			NODE(1, RELATION('>', 1)),
			NODE()
		);
		FST lex_less(
			str,
			2,
			NODE(1, RELATION('<', 1)),
			NODE()
		);
		FST lex_equal(
			str,
			3,
			NODE(1, RELATION('=', 1)),
			NODE(1, RELATION('=', 2)),
			NODE()
		);
		FST lex_exclamation(
			str,
			3,
			NODE(1, RELATION('!', 1)),
			NODE(1, RELATION('=', 2)),
			NODE()
		);
		FST lex_lessEqual(
			str,
			3,
			NODE(1, RELATION('<', 1)),
			NODE(1, RELATION('=', 2)),
			NODE()
		);
		FST lex_moreEqual(
			str,
			3,
			NODE(1, RELATION('>', 1)),
			NODE(1, RELATION('=', 2)),
			NODE()
		);



		FST lex_if(
			str,
			3,
			NODE(1, RELATION('i', 1)),
			NODE(1, RELATION('f', 2)),
			NODE()
		);

		FST lex_else(
			str,
			5,
			NODE(1, RELATION('e', 1)),
			NODE(1, RELATION('l', 2)),
			NODE(1, RELATION('s', 3)),
			NODE(1, RELATION('e', 4)),
			NODE()
		);

		FST lex_id(
			str,
			3,
			NODE(106,
				RELATION('a', 1),
				RELATION('b', 1),
				RELATION('c', 1),
				RELATION('d', 1),
				RELATION('e', 1),
				RELATION('f', 1),
				RELATION('g', 1),
				RELATION('h', 1),
				RELATION('i', 1),
				RELATION('j', 1),
				RELATION('k', 1),
				RELATION('l', 1),
				RELATION('m', 1),
				RELATION('n', 1),
				RELATION('o', 1),
				RELATION('p', 1),
				RELATION('q', 1),
				RELATION('r', 1),
				RELATION('s', 1),
				RELATION('t', 1),
				RELATION('u', 1),
				RELATION('v', 1),
				RELATION('w', 1),
				RELATION('x', 1),
				RELATION('y', 1),
				RELATION('z', 1),
				RELATION('A', 1),
				RELATION('B', 1),
				RELATION('C', 1),
				RELATION('D', 1),
				RELATION('E', 1),
				RELATION('F', 1),
				RELATION('G', 1),
				RELATION('H', 1),
				RELATION('I', 1),
				RELATION('J', 1),
				RELATION('K', 1),
				RELATION('L', 1),
				RELATION('M', 1),
				RELATION('N', 1),
				RELATION('O', 1),
				RELATION('P', 1),
				RELATION('Q', 1),
				RELATION('R', 1),
				RELATION('S', 1),
				RELATION('T', 1),
				RELATION('U', 1),
				RELATION('V', 1),
				RELATION('W', 1),
				RELATION('X', 1),
				RELATION('Y', 1),
				RELATION('Z', 1),
				RELATION('_', 1),
				RELATION('a', 2),
				RELATION('b', 2),
				RELATION('c', 2),
				RELATION('d', 2),
				RELATION('e', 2),
				RELATION('f', 2),
				RELATION('g', 2),
				RELATION('h', 2),
				RELATION('i', 2),
				RELATION('j', 2),
				RELATION('k', 2),
				RELATION('l', 2),
				RELATION('m', 2),
				RELATION('n', 2),
				RELATION('o', 2),
				RELATION('p', 2),
				RELATION('q', 2),
				RELATION('r', 2),
				RELATION('s', 2),
				RELATION('t', 2),
				RELATION('u', 2),
				RELATION('v', 2),
				RELATION('w', 2),
				RELATION('x', 2),
				RELATION('y', 2),
				RELATION('z', 2),
				RELATION('A', 2),
				RELATION('B', 2),
				RELATION('C', 2),
				RELATION('D', 2),
				RELATION('E', 2),
				RELATION('F', 2),
				RELATION('G', 2),
				RELATION('H', 2),
				RELATION('I', 2),
				RELATION('J', 2),
				RELATION('K', 2),
				RELATION('L', 2),
				RELATION('M', 2),
				RELATION('N', 2),
				RELATION('O', 2),
				RELATION('P', 2),
				RELATION('Q', 2),
				RELATION('R', 2),
				RELATION('S', 2),
				RELATION('T', 2),
				RELATION('U', 2),
				RELATION('V', 2),
				RELATION('W', 2),
				RELATION('X', 2),
				RELATION('Y', 2),
				RELATION('Z', 2),
				RELATION('_', 2)
			),
			NODE(126,
				RELATION('a', 1),
				RELATION('b', 1),
				RELATION('c', 1),
				RELATION('d', 1),
				RELATION('e', 1),
				RELATION('f', 1),
				RELATION('g', 1),
				RELATION('h', 1),
				RELATION('i', 1),
				RELATION('j', 1),
				RELATION('k', 1),
				RELATION('l', 1),
				RELATION('m', 1),
				RELATION('n', 1),
				RELATION('o', 1),
				RELATION('p', 1),
				RELATION('q', 1),
				RELATION('r', 1),
				RELATION('s', 1),
				RELATION('t', 1),
				RELATION('u', 1),
				RELATION('v', 1),
				RELATION('w', 1),
				RELATION('x', 1),
				RELATION('y', 1),
				RELATION('z', 1),
				RELATION('A', 1),
				RELATION('B', 1),
				RELATION('C', 1),
				RELATION('D', 1),
				RELATION('E', 1),
				RELATION('F', 1),
				RELATION('G', 1),
				RELATION('H', 1),
				RELATION('I', 1),
				RELATION('J', 1),
				RELATION('K', 1),
				RELATION('L', 1),
				RELATION('M', 1),
				RELATION('N', 1),
				RELATION('O', 1),
				RELATION('P', 1),
				RELATION('Q', 1),
				RELATION('R', 1),
				RELATION('S', 1),
				RELATION('T', 1),
				RELATION('U', 1),
				RELATION('V', 1),
				RELATION('W', 1),
				RELATION('X', 1),
				RELATION('Y', 1),
				RELATION('Z', 1),
				RELATION('_', 1),
				RELATION('0', 1),
				RELATION('1', 1),
				RELATION('2', 1),
				RELATION('3', 1),
				RELATION('4', 1),
				RELATION('5', 1),
				RELATION('6', 1),
				RELATION('7', 1),
				RELATION('8', 1),
				RELATION('9', 1),
				RELATION('a', 2),
				RELATION('b', 2),
				RELATION('c', 2),
				RELATION('d', 2),
				RELATION('e', 2),
				RELATION('f', 2),
				RELATION('g', 2),
				RELATION('h', 2),
				RELATION('i', 2),
				RELATION('j', 2),
				RELATION('k', 2),
				RELATION('l', 2),
				RELATION('m', 2),
				RELATION('n', 2),
				RELATION('o', 2),
				RELATION('p', 2),
				RELATION('q', 2),
				RELATION('r', 2),
				RELATION('s', 2),
				RELATION('t', 2),
				RELATION('u', 2),
				RELATION('v', 2),
				RELATION('w', 2),
				RELATION('x', 2),
				RELATION('y', 2),
				RELATION('z', 2),
				RELATION('A', 2),
				RELATION('B', 2),
				RELATION('C', 2),
				RELATION('D', 2),
				RELATION('E', 2),
				RELATION('F', 2),
				RELATION('G', 2),
				RELATION('H', 2),
				RELATION('I', 2),
				RELATION('J', 2),
				RELATION('K', 2),
				RELATION('L', 2),
				RELATION('M', 2),
				RELATION('N', 2),
				RELATION('O', 2),
				RELATION('P', 2),
				RELATION('Q', 2),
				RELATION('R', 2),
				RELATION('S', 2),
				RELATION('T', 2),
				RELATION('U', 2),
				RELATION('V', 2),
				RELATION('W', 2),
				RELATION('X', 2),
				RELATION('Y', 2),
				RELATION('Z', 2),
				RELATION('_', 2),
				RELATION('0', 2),
				RELATION('1', 2),
				RELATION('2', 2),
				RELATION('3', 2),
				RELATION('4', 2),
				RELATION('5', 2),
				RELATION('6', 2),
				RELATION('7', 2),
				RELATION('8', 2),
				RELATION('9', 2)
			),
			NODE()
		);

		FST lex_function(
			str,
			9,
			NODE(1, RELATION('f', 1)),
			NODE(1, RELATION('u', 2)),
			NODE(1, RELATION('n', 3)),
			NODE(1, RELATION('c', 4)),
			NODE(1, RELATION('t', 5)),
			NODE(1, RELATION('i', 6)),
			NODE(1, RELATION('o', 7)),
			NODE(1, RELATION('n', 8)),
			NODE()
		);

		FST lex_textLiteral(
			str,
			3, 
			NODE(1, RELATION('\"', 1)),
			NODE(256,
				RELATION('\"', 2),
				RELATION((char)0, 1),
				RELATION((char)1, 1),
				RELATION((char)2, 1),
				RELATION((char)3, 1),
				RELATION((char)4, 1),
				RELATION((char)5, 1),
				RELATION((char)6, 1),
				RELATION((char)7, 1),
				RELATION((char)8, 1),
				RELATION((char)9, 1),
				RELATION((char)10, 1),
				RELATION((char)11, 1),
				RELATION((char)12, 1),
				RELATION((char)13, 1),
				RELATION((char)14, 1),
				RELATION((char)15, 1),
				RELATION((char)16, 1),
				RELATION((char)17, 1),
				RELATION((char)18, 1),
				RELATION((char)19, 1),
				RELATION((char)20, 1),
				RELATION((char)21, 1),
				RELATION((char)22, 1),
				RELATION((char)23, 1),
				RELATION((char)24, 1),
				RELATION((char)25, 1),
				RELATION((char)26, 1),
				RELATION((char)27, 1),
				RELATION((char)28, 1),
				RELATION((char)29, 1),
				RELATION((char)30, 1),
				RELATION((char)31, 1),
				RELATION((char)32, 1),
				RELATION((char)33, 1),
				RELATION((char)35, 1),
				RELATION((char)36, 1),
				RELATION((char)37, 1),
				RELATION((char)38, 1),
				RELATION((char)39, 1),
				RELATION((char)40, 1),
				RELATION((char)41, 1),
				RELATION((char)42, 1),
				RELATION((char)43, 1),
				RELATION((char)44, 1),
				RELATION((char)45, 1),
				RELATION((char)46, 1),
				RELATION((char)47, 1),
				RELATION((char)48, 1),
				RELATION((char)49, 1),
				RELATION((char)50, 1),
				RELATION((char)51, 1),
				RELATION((char)52, 1),
				RELATION((char)53, 1),
				RELATION((char)54, 1),
				RELATION((char)55, 1),
				RELATION((char)56, 1),
				RELATION((char)57, 1),
				RELATION((char)58, 1),
				RELATION((char)59, 1),
				RELATION((char)60, 1),
				RELATION((char)61, 1),
				RELATION((char)62, 1),
				RELATION((char)63, 1),
				RELATION((char)64, 1),
				RELATION((char)65, 1),
				RELATION((char)66, 1),
				RELATION((char)67, 1),
				RELATION((char)68, 1),
				RELATION((char)69, 1),
				RELATION((char)70, 1),
				RELATION((char)71, 1),
				RELATION((char)72, 1),
				RELATION((char)73, 1),
				RELATION((char)74, 1),
				RELATION((char)75, 1),
				RELATION((char)76, 1),
				RELATION((char)77, 1),
				RELATION((char)78, 1),
				RELATION((char)79, 1),
				RELATION((char)80, 1),
				RELATION((char)81, 1),
				RELATION((char)82, 1),
				RELATION((char)83, 1),
				RELATION((char)84, 1),
				RELATION((char)85, 1),
				RELATION((char)86, 1),
				RELATION((char)87, 1),
				RELATION((char)88, 1),
				RELATION((char)89, 1),
				RELATION((char)90, 1),
				RELATION((char)91, 1),
				RELATION((char)92, 1),
				RELATION((char)93, 1),
				RELATION((char)94, 1),
				RELATION((char)95, 1),
				RELATION((char)96, 1),
				RELATION((char)97, 1),
				RELATION((char)98, 1),
				RELATION((char)99, 1),
				RELATION((char)100, 1),
				RELATION((char)101, 1),
				RELATION((char)102, 1),
				RELATION((char)103, 1),
				RELATION((char)104, 1),
				RELATION((char)105, 1),
				RELATION((char)106, 1),
				RELATION((char)107, 1),
				RELATION((char)108, 1),
				RELATION((char)109, 1),
				RELATION((char)110, 1),
				RELATION((char)111, 1),
				RELATION((char)112, 1),
				RELATION((char)113, 1),
				RELATION((char)114, 1),
				RELATION((char)115, 1),
				RELATION((char)116, 1),
				RELATION((char)117, 1),
				RELATION((char)118, 1),
				RELATION((char)119, 1),
				RELATION((char)120, 1),
				RELATION((char)121, 1),
				RELATION((char)122, 1),
				RELATION((char)123, 1),
				RELATION((char)124, 1),
				RELATION((char)125, 1),
				RELATION((char)126, 1),
				RELATION((char)127, 1),
				RELATION((char)128, 1),
				RELATION((char)129, 1),
				RELATION((char)130, 1),
				RELATION((char)131, 1),
				RELATION((char)132, 1),
				RELATION((char)133, 1),
				RELATION((char)134, 1),
				RELATION((char)135, 1),
				RELATION((char)136, 1),
				RELATION((char)137, 1),
				RELATION((char)138, 1),
				RELATION((char)139, 1),
				RELATION((char)140, 1),
				RELATION((char)141, 1),
				RELATION((char)142, 1),
				RELATION((char)143, 1),
				RELATION((char)144, 1),
				RELATION((char)145, 1),
				RELATION((char)146, 1),
				RELATION((char)147, 1),
				RELATION((char)148, 1),
				RELATION((char)149, 1),
				RELATION((char)150, 1),
				RELATION((char)151, 1),
				RELATION((char)152, 1),
				RELATION((char)153, 1),
				RELATION((char)154, 1),
				RELATION((char)155, 1),
				RELATION((char)156, 1),
				RELATION((char)157, 1),
				RELATION((char)158, 1),
				RELATION((char)159, 1),
				RELATION((char)160, 1),
				RELATION((char)161, 1),
				RELATION((char)162, 1),
				RELATION((char)163, 1),
				RELATION((char)164, 1),
				RELATION((char)165, 1),
				RELATION((char)166, 1),
				RELATION((char)167, 1),
				RELATION((char)168, 1),
				RELATION((char)169, 1),
				RELATION((char)170, 1),
				RELATION((char)171, 1),
				RELATION((char)172, 1),
				RELATION((char)173, 1),
				RELATION((char)174, 1),
				RELATION((char)175, 1),
				RELATION((char)176, 1),
				RELATION((char)177, 1),
				RELATION((char)178, 1),
				RELATION((char)179, 1),
				RELATION((char)180, 1),
				RELATION((char)181, 1),
				RELATION((char)182, 1),
				RELATION((char)183, 1),
				RELATION((char)184, 1),
				RELATION((char)185, 1),
				RELATION((char)186, 1),
				RELATION((char)187, 1),
				RELATION((char)188, 1),
				RELATION((char)189, 1),
				RELATION((char)190, 1),
				RELATION((char)191, 1),
				RELATION((char)192, 1),
				RELATION((char)193, 1),
				RELATION((char)194, 1),
				RELATION((char)195, 1),
				RELATION((char)196, 1),
				RELATION((char)197, 1),
				RELATION((char)198, 1),
				RELATION((char)199, 1),
				RELATION((char)200, 1),
				RELATION((char)201, 1),
				RELATION((char)202, 1),
				RELATION((char)203, 1),
				RELATION((char)204, 1),
				RELATION((char)205, 1),
				RELATION((char)206, 1),
				RELATION((char)207, 1),
				RELATION((char)208, 1),
				RELATION((char)209, 1),
				RELATION((char)210, 1),
				RELATION((char)211, 1),
				RELATION((char)212, 1),
				RELATION((char)213, 1),
				RELATION((char)214, 1),
				RELATION((char)215, 1),
				RELATION((char)216, 1),
				RELATION((char)217, 1),
				RELATION((char)218, 1),
				RELATION((char)219, 1),
				RELATION((char)220, 1),
				RELATION((char)221, 1),
				RELATION((char)222, 1),
				RELATION((char)223, 1),
				RELATION((char)224, 1),
				RELATION((char)225, 1),
				RELATION((char)226, 1),
				RELATION((char)227, 1),
				RELATION((char)228, 1),
				RELATION((char)229, 1),
				RELATION((char)230, 1),
				RELATION((char)231, 1),
				RELATION((char)232, 1),
				RELATION((char)233, 1),
				RELATION((char)234, 1),
				RELATION((char)235, 1),
				RELATION((char)236, 1),
				RELATION((char)237, 1),
				RELATION((char)238, 1),
				RELATION((char)239, 1),
				RELATION((char)240, 1),
				RELATION((char)241, 1),
				RELATION((char)242, 1),
				RELATION((char)243, 1),
				RELATION((char)244, 1),
				RELATION((char)245, 1),
				RELATION((char)246, 1),
				RELATION((char)247, 1),
				RELATION((char)248, 1),
				RELATION((char)249, 1),
				RELATION((char)250, 1),
				RELATION((char)251, 1),
				RELATION((char)252, 1),
				RELATION((char)253, 1),
				RELATION((char)254, 1),
				RELATION((char)255, 1)
			),
			NODE()
		);

		FST lex_symbolLiteral(
			str,
			3,
			NODE(1, RELATION('\'', 1)),
			NODE(256,
				RELATION('\'', 2),
				RELATION((char)0, 1),
				RELATION((char)1, 1),
				RELATION((char)2, 1),
				RELATION((char)3, 1),
				RELATION((char)4, 1),
				RELATION((char)5, 1),
				RELATION((char)6, 1),
				RELATION((char)7, 1),
				RELATION((char)8, 1),
				RELATION((char)9, 1),
				RELATION((char)10, 1),
				RELATION((char)11, 1),
				RELATION((char)12, 1),
				RELATION((char)13, 1),
				RELATION((char)14, 1),
				RELATION((char)15, 1),
				RELATION((char)16, 1),
				RELATION((char)17, 1),
				RELATION((char)18, 1),
				RELATION((char)19, 1),
				RELATION((char)20, 1),
				RELATION((char)21, 1),
				RELATION((char)22, 1),
				RELATION((char)23, 1),
				RELATION((char)24, 1),
				RELATION((char)25, 1),
				RELATION((char)26, 1),
				RELATION((char)27, 1),
				RELATION((char)28, 1),
				RELATION((char)29, 1),
				RELATION((char)30, 1),
				RELATION((char)31, 1),
				RELATION((char)32, 1),
				RELATION((char)33, 1),
				RELATION((char)35, 1),
				RELATION((char)36, 1),
				RELATION((char)37, 1),
				RELATION((char)38, 1),
				RELATION((char)39, 1),
				RELATION((char)40, 1),
				RELATION((char)41, 1),
				RELATION((char)42, 1),
				RELATION((char)43, 1),
				RELATION((char)44, 1),
				RELATION((char)45, 1),
				RELATION((char)46, 1),
				RELATION((char)47, 1),
				RELATION((char)48, 1),
				RELATION((char)49, 1),
				RELATION((char)50, 1),
				RELATION((char)51, 1),
				RELATION((char)52, 1),
				RELATION((char)53, 1),
				RELATION((char)54, 1),
				RELATION((char)55, 1),
				RELATION((char)56, 1),
				RELATION((char)57, 1),
				RELATION((char)58, 1),
				RELATION((char)59, 1),
				RELATION((char)60, 1),
				RELATION((char)61, 1),
				RELATION((char)62, 1),
				RELATION((char)63, 1),
				RELATION((char)64, 1),
				RELATION((char)65, 1),
				RELATION((char)66, 1),
				RELATION((char)67, 1),
				RELATION((char)68, 1),
				RELATION((char)69, 1),
				RELATION((char)70, 1),
				RELATION((char)71, 1),
				RELATION((char)72, 1),
				RELATION((char)73, 1),
				RELATION((char)74, 1),
				RELATION((char)75, 1),
				RELATION((char)76, 1),
				RELATION((char)77, 1),
				RELATION((char)78, 1),
				RELATION((char)79, 1),
				RELATION((char)80, 1),
				RELATION((char)81, 1),
				RELATION((char)82, 1),
				RELATION((char)83, 1),
				RELATION((char)84, 1),
				RELATION((char)85, 1),
				RELATION((char)86, 1),
				RELATION((char)87, 1),
				RELATION((char)88, 1),
				RELATION((char)89, 1),
				RELATION((char)90, 1),
				RELATION((char)91, 1),
				RELATION((char)92, 1),
				RELATION((char)93, 1),
				RELATION((char)94, 1),
				RELATION((char)95, 1),
				RELATION((char)96, 1),
				RELATION((char)97, 1),
				RELATION((char)98, 1),
				RELATION((char)99, 1),
				RELATION((char)100, 1),
				RELATION((char)101, 1),
				RELATION((char)102, 1),
				RELATION((char)103, 1),
				RELATION((char)104, 1),
				RELATION((char)105, 1),
				RELATION((char)106, 1),
				RELATION((char)107, 1),
				RELATION((char)108, 1),
				RELATION((char)109, 1),
				RELATION((char)110, 1),
				RELATION((char)111, 1),
				RELATION((char)112, 1),
				RELATION((char)113, 1),
				RELATION((char)114, 1),
				RELATION((char)115, 1),
				RELATION((char)116, 1),
				RELATION((char)117, 1),
				RELATION((char)118, 1),
				RELATION((char)119, 1),
				RELATION((char)120, 1),
				RELATION((char)121, 1),
				RELATION((char)122, 1),
				RELATION((char)123, 1),
				RELATION((char)124, 1),
				RELATION((char)125, 1),
				RELATION((char)126, 1),
				RELATION((char)127, 1),
				RELATION((char)128, 1),
				RELATION((char)129, 1),
				RELATION((char)130, 1),
				RELATION((char)131, 1),
				RELATION((char)132, 1),
				RELATION((char)133, 1),
				RELATION((char)134, 1),
				RELATION((char)135, 1),
				RELATION((char)136, 1),
				RELATION((char)137, 1),
				RELATION((char)138, 1),
				RELATION((char)139, 1),
				RELATION((char)140, 1),
				RELATION((char)141, 1),
				RELATION((char)142, 1),
				RELATION((char)143, 1),
				RELATION((char)144, 1),
				RELATION((char)145, 1),
				RELATION((char)146, 1),
				RELATION((char)147, 1),
				RELATION((char)148, 1),
				RELATION((char)149, 1),
				RELATION((char)150, 1),
				RELATION((char)151, 1),
				RELATION((char)152, 1),
				RELATION((char)153, 1),
				RELATION((char)154, 1),
				RELATION((char)155, 1),
				RELATION((char)156, 1),
				RELATION((char)157, 1),
				RELATION((char)158, 1),
				RELATION((char)159, 1),
				RELATION((char)160, 1),
				RELATION((char)161, 1),
				RELATION((char)162, 1),
				RELATION((char)163, 1),
				RELATION((char)164, 1),
				RELATION((char)165, 1),
				RELATION((char)166, 1),
				RELATION((char)167, 1),
				RELATION((char)168, 1),
				RELATION((char)169, 1),
				RELATION((char)170, 1),
				RELATION((char)171, 1),
				RELATION((char)172, 1),
				RELATION((char)173, 1),
				RELATION((char)174, 1),
				RELATION((char)175, 1),
				RELATION((char)176, 1),
				RELATION((char)177, 1),
				RELATION((char)178, 1),
				RELATION((char)179, 1),
				RELATION((char)180, 1),
				RELATION((char)181, 1),
				RELATION((char)182, 1),
				RELATION((char)183, 1),
				RELATION((char)184, 1),
				RELATION((char)185, 1),
				RELATION((char)186, 1),
				RELATION((char)187, 1),
				RELATION((char)188, 1),
				RELATION((char)189, 1),
				RELATION((char)190, 1),
				RELATION((char)191, 1),
				RELATION((char)192, 1),
				RELATION((char)193, 1),
				RELATION((char)194, 1),
				RELATION((char)195, 1),
				RELATION((char)196, 1),
				RELATION((char)197, 1),
				RELATION((char)198, 1),
				RELATION((char)199, 1),
				RELATION((char)200, 1),
				RELATION((char)201, 1),
				RELATION((char)202, 1),
				RELATION((char)203, 1),
				RELATION((char)204, 1),
				RELATION((char)205, 1),
				RELATION((char)206, 1),
				RELATION((char)207, 1),
				RELATION((char)208, 1),
				RELATION((char)209, 1),
				RELATION((char)210, 1),
				RELATION((char)211, 1),
				RELATION((char)212, 1),
				RELATION((char)213, 1),
				RELATION((char)214, 1),
				RELATION((char)215, 1),
				RELATION((char)216, 1),
				RELATION((char)217, 1),
				RELATION((char)218, 1),
				RELATION((char)219, 1),
				RELATION((char)220, 1),
				RELATION((char)221, 1),
				RELATION((char)222, 1),
				RELATION((char)223, 1),
				RELATION((char)224, 1),
				RELATION((char)225, 1),
				RELATION((char)226, 1),
				RELATION((char)227, 1),
				RELATION((char)228, 1),
				RELATION((char)229, 1),
				RELATION((char)230, 1),
				RELATION((char)231, 1),
				RELATION((char)232, 1),
				RELATION((char)233, 1),
				RELATION((char)234, 1),
				RELATION((char)235, 1),
				RELATION((char)236, 1),
				RELATION((char)237, 1),
				RELATION((char)238, 1),
				RELATION((char)239, 1),
				RELATION((char)240, 1),
				RELATION((char)241, 1),
				RELATION((char)242, 1),
				RELATION((char)243, 1),
				RELATION((char)244, 1),
				RELATION((char)245, 1),
				RELATION((char)246, 1),
				RELATION((char)247, 1),
				RELATION((char)248, 1),
				RELATION((char)249, 1),
				RELATION((char)250, 1),
				RELATION((char)251, 1),
				RELATION((char)252, 1),
				RELATION((char)253, 1),
				RELATION((char)254, 1),
				RELATION((char)255, 1)
			),
			NODE()
		);

		FST lex_logicalLiteral(
			str,
			2,
			NODE(2,
				RELATION('0', 0),
				RELATION('1', 0)
			),
			NODE()
		);

		FST lex_byteLiteral(
			str,
			2,
			NODE(20,
				RELATION('0', 0),
				RELATION('1', 0),
				RELATION('2', 0),
				RELATION('3', 0),
				RELATION('4', 0),
				RELATION('5', 0),
				RELATION('6', 0),
				RELATION('7', 0),
				RELATION('8', 0),
				RELATION('9', 0),
				RELATION('0', 1),
				RELATION('1', 1),
				RELATION('2', 1),
				RELATION('3', 1),
				RELATION('4', 1),
				RELATION('5', 1),
				RELATION('6', 1),
				RELATION('7', 1),
				RELATION('8', 1),
				RELATION('9', 1)
			),
			NODE()
		);
		
		FSTAssigned FSTarray[] = {
			FSTAssigned(&lex_byte, IT::BYTE, LEX_BYTE),							// 0
			FSTAssigned(&lex_text, IT::TEXT, LEX_TEXT),							// 1
			FSTAssigned(&lex_symbol, IT::SYMBOL, LEX_SYMBOL),					// 2
			FSTAssigned(&lex_function, (IT::IDDATATYPE)0, LEX_FUNCTION),        // 3
			FSTAssigned(&lex_declare, (IT::IDDATATYPE)0, LEX_DECLARE),          // 4
			FSTAssigned(&lex_return, (IT::IDDATATYPE)0, LEX_RETURN),			// 5
			FSTAssigned(&lex_display, (IT::IDDATATYPE)0, LEX_DISPLAY),			// 6
			FSTAssigned(&lex_main, (IT::IDDATATYPE)0, LEX_MAIN),				// 7

			FSTAssigned(&lex_semicolon, (IT::IDDATATYPE)0, LEX_SEMICOLON),		// 8
			FSTAssigned(&lex_comma, (IT::IDDATATYPE)0, LEX_COMMA),				// 9
			FSTAssigned(&lex_leftbrace, (IT::IDDATATYPE)0, LEX_LEFTBRACE),		// 10
			FSTAssigned(&lex_bracelet, (IT::IDDATATYPE)0, LEX_BRACELET),		// 11
			FSTAssigned(&lex_leftthesis, (IT::IDDATATYPE)0, LEX_LEFTTHESIS),	// 12
			FSTAssigned(&lex_rightthesis, (IT::IDDATATYPE)0, LEX_RIGHTTHESIS),  // 13

			FSTAssigned(&lex_plus, (IT::IDDATATYPE)0, LEX_PLUS),				// 14
			FSTAssigned(&lex_minus, (IT::IDDATATYPE)0, LEX_MINUS),				// 15
			FSTAssigned(&lex_start, (IT::IDDATATYPE)0, LEX_START),				// 16
			FSTAssigned(&lex_dirslash, (IT::IDDATATYPE)0, LEX_DIRSLASH),		// 17

			FSTAssigned(&lex_assigment, (IT::IDDATATYPE)0, LEX_ASSIGNMENT),	    // 18

			FSTAssigned(&lex_if, (IT::IDDATATYPE)0, LEX_IF),					// 19
			FSTAssigned(&lex_else, (IT::IDDATATYPE)0, LEX_ELSE),				// 20

			FSTAssigned(&lex_more, (IT::IDDATATYPE)0, LEX_MORE),				// 21
			FSTAssigned(&lex_less, (IT::IDDATATYPE)0, LEX_LESS),				// 22
			FSTAssigned(&lex_equal, (IT::IDDATATYPE)0, LEX_EQUAL),				// 23
			FSTAssigned(&lex_exclamation, (IT::IDDATATYPE)0, LEX_EXCLAMATION),  // 24
			FSTAssigned(&lex_moreEqual, (IT::IDDATATYPE)0, LEX_MOREEQUAL),      // 25
			FSTAssigned(&lex_lessEqual, (IT::IDDATATYPE)0, LEX_LESSEQUAL),      // 26

			FSTAssigned(&lex_id, (IT::IDDATATYPE)0, LEX_ID),					// 27
			FSTAssigned(&lex_textLiteral, IT::TEXT, LEX_LITERAL),				// 28
			FSTAssigned(&lex_symbolLiteral, IT::SYMBOL, LEX_LITERAL),			// 29
			FSTAssigned(&lex_byteLiteral, IT::BYTE, LEX_LITERAL),				// 30
			FSTAssigned(&lex_logicalLiteral, IT::BOOLEAN, LEX_BOOLEAN)			// 31
		};
		
		int FSTarrayLen = 32;

		IT::IDDATATYPE iddatatype = (IT::IDDATATYPE)0;
		IT::IDTYPE idtype = (IT::IDTYPE)0;

		int numberOfMainDefine = 0;
		int numberOfLexems = 0;
		bool inLiteral = false;
		char expectedSymbol;
		bool isPrevWasStopSymbol = false;

		char ch;
		int line = 0;
		std::vector<char>word;
		
 		for (int i = 0; i < in.size; i++) {
			ch = in.text[i];

			if (ch == '\n') {
				line += 1;
				i += 1;
			}

			if (isSymbolIsStopSymbol(ch)) {
				if (ch == '\'' || ch == '\"') {
					if (inLiteral) {
						if (ch == expectedSymbol) {
							inLiteral = false;
							word.push_back(ch);
							str.clear();
							str = word;
							// кинуть в execute
							show(word);
							word.clear();
						}
						else {
							throw ERROR_THROW(126);
						}
					}
					else {
						inLiteral = true;
						expectedSymbol = ch;
						word.push_back(ch);
					}
				}
				else {
					if (inLiteral) {
						word.push_back(ch);
					}
					else {
						str.clear();
						str = word;
						// кидаем в execute
						show(word);
						word.clear();
					}
				}
			}
			else {
				word.push_back(ch);
			}
		}
	}
}