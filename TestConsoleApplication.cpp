#include <iostream>
#include <string>

// 定义部分常量
#define MAX_ARRAY_SIZE 125  // 包含118个元素和一些额外的空间用于缓冲
#define CHEMICALS_LENGTH 118  // 总共包含118个元素
#define END_OF_ARRAY_MARKER '*'  // 用于标记数组结束的特殊字符

// 该程序用于计算化学式的分子量, 输入化学式后按回车键即可得到分子量, 输入 "exit", "quit" 或 "end" 来退出程序
using namespace std;
extern string chemicals;
extern const string union_chemical_elements[MAX_ARRAY_SIZE];
extern string temp_elements;
extern string count_string;
extern const uint16_t union_chemicals_mass[MAX_ARRAY_SIZE];
extern uint8_t right_move_count;
extern int i;
extern bool success_signal;
extern bool post_signal;
extern bool bracket_signal;
extern unsigned int bracket_mass_counter;
extern unsigned int total_mass;

// 定义化学元素符号和对应的原子量数组，以及加上一大堆的额外空间用于缓冲，避免在循环中访问越界。数组中的元素符号和原子量按照元素周期表的顺序排列，直到第118个元素（Og）。数组末尾使用特殊字符 "*" 和0来标记结束，以便在循环中进行验证时能够正确识别化学式的结束。
const string union_chemical_elements[MAX_ARRAY_SIZE] = {
	"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne",
	"Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar",
	"K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe",
	"Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As",
	"Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc",
	"Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te",
	"I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm",
	"Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
	"Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au",
	"Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra",
	"Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk",
	"Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg",
	"Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc",
	"Lv", "Ts", "Og",

	// Sentinel value for end of array
	"*", "*", "*", "*", "*", "*", "*"
};

// 定义化学元素的原子量数组，按照元素周期表的顺序排列，直到第118个元素（Og）。数组末尾使用0来标记结束，以便在循环中进行验证时能够正确识别化学式的结束。
const uint16_t union_chemicals_mass[MAX_ARRAY_SIZE] = {
    1,    // H,  氢
    4,    // He, 氦
    7,    // Li, 锂
    9,    // Be, 铍
    11,   // B,  硼
    12,   // C,  碳
    14,   // N,  氮
    16,   // O,  氧
    19,   // F,  氟
    20,   // Ne, 氖
    23,   // Na, 钠
    24,   // Mg, 镁
    27,   // Al, 铝
    28,   // Si, 硅
    31,   // P,  磷
    32,   // S,  硫
    35,   // Cl, 氯
    40,   // Ar, 氩
    39,   // K,  钾
    40,   // Ca, 钙
    45,   // Sc, 钪
    48,   // Ti, 钛
    51,   // V,  钒
    52,   // Cr, 铬
    55,   // Mn, 锰
    56,   // Fe, 铁
    59,   // Co, 钴
    59,   // Ni, 镍
    64,   // Cu, 铜
    65,   // Zn, 锌
    70,   // Ga, 镓
    73,   // Ge, 锗
    75,   // As, 砷
    79,   // Se, 硒
    80,   // Br, 溴
    84,   // Kr, 氪
    85,   // Rb, 铷
    88,   // Sr, 锶
    89,   // Y,  钇
    91,   // Zr, 锆
    93,   // Nb, 铌
    96,   // Mo, 钼
    98,   // Tc, 锝
    101,  // Ru, 钌
    103,  // Rh, 铑
    106,  // Pd, 钯
    108,  // Ag, 银
    112,  // Cd, 镉
    115,  // In, 铟
    119,  // Sn, 锡
    122,  // Sb, 锑
    128,  // Te, 碲
    127,  // I,  碘
    131,  // Xe, 氙
    133,  // Cs, 铯
    137,  // Ba, 钡
    139,  // La, 镧
    140,  // Ce, 铈
    141,  // Pr, 镨
    144,  // Nd, 钕
    145,  // Pm, 钷
    150,  // Sm, 钐
    152,  // Eu, 铕
    157,  // Gd, 钆
    159,  // Tb, 铽
    163,  // Dy, 镝
    165,  // Ho, 钬
    167,  // Er, 铒
    169,  // Tm, 铥
    173,  // Yb, 镱
    175,  // Lu, 镥
    178,  // Hf, 铪
    181,  // Ta, 钽
    184,  // W,  钨
    186,  // Re, 铼
    190,  // Os, 锇
    192,  // Ir, 铱
    195,  // Pt, 铂
    197,  // Au, 金
    201,  // Hg, 汞
    204,  // Tl, 铊
    207,  // Pb, 铅
    209,  // Bi, 铋
    209,  // Po, 钋
    210,  // At, 砹
    222,  // Rn, 氡
    223,  // Fr, 钫
    226,  // Ra, 镭
    227,  // Ac, 锕
    232,  // Th, 钍
    231,  // Pa, 镤
    238,  // U,  铀
    237,  // Np, 镎
    244,  // Pu, 钚
    243,  // Am, 镅
    247,  // Cm, 锔
    247,  // Bk, 锫
    251,  // Cf, 锎
    252,  // Es, 锿
    257,  // Fm, 镄
    258,  // Md, 钔
    259,  // No, 锘
    262,  // Lr, 铹
    267,  // Rf, 𬬻
    268,  // Db, 𬭊
    271,  // Sg, 𬭳
    274,  // Bh, 𬭛
    269,  // Hs, 𬭶
    276,  // Mt, 鿏
    281,  // Ds, 𫟼
    282,  // Rg, 𬬭
    285,  // Cn, 鎶
    286,  // Nh, 鉨
    289,  // Fl, 𫓧
    290,  // Mc, 镆
    293,  // Lv, 𫟷
    294,  // Ts, 鿬
    294,   // Og, 鿫 [citation:10]

    // Sentinel value for end of array
	0, 0, 0, 0, 0, 0, 0
};

// 定义主函数
int main() {
	// 输出欢迎信息和使用说明，使用绿色的 "[+]" 来标记提示信息，使用红色的 "[-]" 来标记错误信息，以增强用户体验和界面友好性。
	cout << "\033[34m[*]\033[0m Welcome to the Chemical Formula Molecular Mass Calculator!" << endl;
    cout << "\033[34m[*]\033[0m Please enter a chemical formula to calculate its molecular mass." << endl;
	cout << "\033[34m[*]\033[0m Also, you can enter 'exit', 'quit', or 'end' to exit the program." << endl;
	cout << "\033[34m[*]\033[0m The chemical formula should only contain valid element symbols (e.g., H, He, Li) followed by optional numbers (e.g., H2, O2)." << endl;
	cout << "\033[34m[*]\033[0m For example, you can enter 'H2O' to calculate the molecular mass of water." << endl;
	cout << "\033[34m[*]\033[0m If you need to type a repeating part, you can use bracket to do it. Like: Cu(OH)2." << endl;
	cout << "\033[34m[*]\033[0m Because approximate values were used instead of accurate ones, it is inevitable that there will be minor errors." << endl;
	cout << "\033[34m[*]\033[0m Let's get started!" << endl << endl;

    while (true) {  // 无限循环，直到用户输入 "exit", "quit" 或 "end" 来退出程序
        // 存储用户输入的化学式，并且进行初步判断是否为特殊的退出命令或者空输入
        string chemicals;
        cout << "input: ";
        cin >> chemicals;

		// 使用 erase 删除多余的空格和各种乱七八糟的不可见字符，确保输入的化学式是干净的，避免在后续的验证和计算过程中出现问题。这里使用了 remove-erase惯用法来删除字符串中的空格字符。
        chemicals.erase(remove(chemicals.begin(), chemicals.end(), ' '), chemicals.end());
		chemicals.erase(remove(chemicals.begin(), chemicals.end(), '\x00'), chemicals.end());

		// 进行一个简单检测，确保所有字符都是ASCII字符，避免用户输入了非ASCII字符导致程序无法正确处理。这里使用了一个循环来检查字符串中的每个字符是否都是ASCII字符，如果发现非ASCII字符，则输出错误信息并跳出循环。
        bool success_signal = true;
        for (char c : chemicals) {
            if (static_cast<unsigned char>(c) > 127) {
                cout << "\033[31m[-]\033[0m Invalid chemical formula: non-ASCII character '" << c << "' detected." << endl;
                success_signal = false;
            }
		}
		if (!success_signal) continue;  // 如果检测到非ASCII字符，则跳出循环，等待用户重新输入化学式。

        // 初步判断用户输入的化学式是否为退出命令或者空输入，亦或者存在手动输入的终止符，如果是则退出程序或者提示用户输入有效的化学式
        if (chemicals == "exit" || chemicals == "quit" || chemicals == "end") {
			cout << "\033[34m[*]\033[0m Exiting the program. Goodbye!" << endl;
            return 0;
        }
        else if (chemicals.length() <= 0) {
            cout << "\033[31m[-]\033[0m Please enter a valid chemical formula." << endl;
            break;
        }
        else if (chemicals.find("*") != string::npos) {
			cout << "\033[31m[-]\033[0m Invalid chemical formula: chemical formula cannot contain the character '*', as it is reserved for internal use." << endl;
            break;
        }

        // 如果输入的化学式不为空且不是退出命令，则进行进一步的验证并尝试计算分子量
        else {
			bool success_signal = false;  // 定义一个布尔变量 success_signal，用于指示化学式验证和分子量计算是否成功。初始值为 false，表示尚未成功。
			bool bracket_signal = false;  // 定义一个布尔变量 bracket_signal，用于指示化学式中是否存在括号。初始值为 false，表示尚未检测到括号。
            unsigned int total_mass = 0;  // 初始化总分子量为0
			unsigned int bracket_mass_counter = 0;  // 定义一个变量 bracket_mass_counter，用于在处理括号内的部分时临时存储该部分的分子量。初始值为0，表示尚未计算任何分子量。

            // 在化学式的末尾添加数个特殊字符 "*"，作为循环验证的结束标志，这样在循环中就不需要额外的条件来判断是否已经到达化学式的末尾。
            chemicals += "******";

            // 开始循环并进行化学式的验证，确保输入的化学式符合化学式的基本规则，例如元素符号必须由一个大写字母开头，后面可能跟一个小写字母（如果元素符号是两字母的），以及可能跟一个数字（表示该元素的数量）。同时还需要检查化学式中是否包含无效字符。
            for (int i = 0; i < chemicals.length(); i++) {

                // 如果成功遍历到特殊字符 "*"，说明化学式验证成功，设置 success_signal 为 true
                if (chemicals[i] == END_OF_ARRAY_MARKER) {
                    success_signal = true;
                    break;
                }

                // 通常而言，化学式中的元素符号由一个大写字母开头，后面可能跟一个小写字母（如果元素符号是两字母的），以及可能跟一个数字（表示该元素的数量）。因此，我们需要检查每个字符是否符合这些规则。
                else if (isupper(chemicals[i])) {
                    string temp_elements;  // 临时字符串，用于存储当前正在解析的元素符号。通常只有1~2个字符。
                    temp_elements = chemicals[i];  // 将当前的大写字母添加到 temp_elements 中，作为元素符号的开头。
                    uint8_t right_move_count = 1;  // 定义或重置变量 right_move_count，用于记录在化学式中向右移动的字符数，以便正确解析元素符号和数量。初始值为1，表示当前已经解析了一个大写字母。

                    // 如果下一个字符是小写字母，那么它是当前元素符号的一部分，将其添加到 temp_elements 中
                    if (chemicals[i + 1] != END_OF_ARRAY_MARKER && islower(chemicals[i + 1])) {
                        temp_elements.push_back(chemicals[i + 1]);  // 将小写字母添加到 temp_elements 中，形成完整的元素符号。
                        right_move_count++;  // 向右移动一个字符，表示已经解析了一个小写字母。
                    }

                    // 定义一个布尔变量 post_signal，用于指示是否成功解析了一个元素符号。初始值为 false，表示尚未成功。
                    bool post_signal = false;

                    // 在 union_chemical_elements 数组中查找 temp_elements，找到后将对应的原子量添加到 total_mass 中。
                    for (int j = 0; j <= CHEMICALS_LENGTH; j++) {
                        if (temp_elements == union_chemical_elements[j]) {

                            // 如果下一个字符是数字，表示该元素的数量。
                            if (chemicals[i + right_move_count] != END_OF_ARRAY_MARKER && isdigit(chemicals[i + right_move_count])) {

                                // 定义一个字符串变量 count_string，用于存储元素数量的字符串形式，以便处理多位数字的情况。
                                string count_string;
								int pointer_count = i + right_move_count;
                                
                                // 定义一个指针变量 pointer_count，用于指向下一个字符的位置，初始值为当前元素符号的末尾位置。
                                while (chemicals[pointer_count] != END_OF_ARRAY_MARKER && isdigit(chemicals[pointer_count])) {
									count_string.push_back(chemicals[pointer_count]);  // 将数字字符添加到 count_string 中，形成完整的数量字符串。
									pointer_count++;  // 向右移动一个字符，继续检查下一个字符是否也是数字。
                                }

								// 将 right_move_count 更新为 pointer_count 与当前元素符号末尾位置的差值，以便在下一次循环中正确解析下一个元素符号。
                                if (bracket_signal) {
									bracket_mass_counter += union_chemicals_mass[j] * stoi(count_string);  // 如果当前在括号内，则将元素的原子量乘以数量（将 count_string 转换为整数），添加到括号内的分子量计数器中，以便在遇到右括号时正确计算括号内部分的分子量。    
                                }
                                else {  // 否则，将元素的原子量乘以数量（将 count_string 转换为整数），添加到总分子量中。
                                    total_mass += union_chemicals_mass[j] * stoi(count_string);
                                }
                            }

                            // 如果下一个字符不是数字，表示该元素的数量默认为1
                            else {
                                if (bracket_signal) {
									bracket_mass_counter += union_chemicals_mass[j];  // 将元素的原子量添加到括号内的分子量计数器中，以便在遇到右括号时正确计算括号内部分的分子量。
                                }
                                else {
                                    total_mass += union_chemicals_mass[j];  // 否则，则直接将元素的原子量添加到总分子量中。
                                }
                            }

                            // 成功解析了一个元素符号，设置 post_signal 为 true
                            post_signal = true;
                            break;
                        }
                    }

                    // 如果没有找到匹配的元素符号，说明化学式无效，输出错误信息并跳出循环。
                    if (!post_signal) {
                        cout << "\033[31m[-]\033[0m Invalid chemical formula: element symbol '" << temp_elements << "' not found." << endl;
                        break;
                    }
                }

                // 如果这是一个小写字母，那么它必须紧跟在一个大写字母后面，形成一个合法的元素符号（例如 "He"）。如果小写字母出现在其他位置或者没有前面的元素符号，那么这就是一个无效的化学式。
                else if (chemicals[i] != END_OF_ARRAY_MARKER && islower(chemicals[i])) {
                    if (i == 0 || !isupper(chemicals[i - 1])) {  // 如果小写字母出现在化学式的开头，或者前面不是一个大写字母，那么这就是一个无效的化学式
                        cout << "\033[31m[-]\033[0m Invalid chemical formula: lowercase letter '" << chemicals[i] << "' must follow an uppercase letter." << endl;
                        break;
                    }
                    continue;
                }

                // 如果这是一个数字，那么它必须紧跟在一个元素符号后面，表示该元素的数量（例如 "H2" 表示两个氢原子）。如果数字出现在其他位置或者没有前面的元素符号，那么这也是一个无效的化学式。
                else if (chemicals[i] != END_OF_ARRAY_MARKER && isdigit(chemicals[i])) {
                    if (i == 0) {  // 如果数字出现在化学式的开头，或者前面不是一个大写字母或小写字母，那么这就是一个无效的化学式
                        cout << "\033[31m[-]\033[0m Invalid chemical formula: digit '" << chemicals[i] << "' must follow an element symbol." << endl;
                        break;
                    }
                    continue;
                }

				// 如果这是一个左括号，那么设置 bracket_signal 为 true，表示进入了一个括号内的部分。括号内的内容需要单独处理，以便正确计算分子量。
                else if (chemicals[i] == '(') {
                    bracket_signal = true;  // 如果遇到左括号，设置 bracket_signal 为 true，表示进入了一个括号内的部分。
                    continue;
                }

				// 如果这是一个右括号，那么之前必须已经遇到过一个左括号，表示这是一个合法的括号对。如果遇到右括号但之前没有遇到过左括号，那么这就是一个无效的化学式。
                else if (chemicals[i] == ')') {
                    if (!bracket_signal) {  // 如果遇到右括号，但之前没有遇到过左括号，那么这就是一个无效的化学式
                        cout << "\033[31m[-]\033[0m Invalid chemical formula: unmatched closing parenthesis." << endl;
                        break;
                    }

                    bracket_signal = false;  // 如果遇到右括号，设置 bracket_signal 为 false，表示离开了括号内的部分。
                    if (isdigit(chemicals[i + 1])) {  // 如果右括号后面跟着一个数字，表示括号内的部分需要乘以这个数量。
						total_mass += bracket_mass_counter * (chemicals[i + 1] - '0');  // 将括号内的分子量乘以数量（将字符数字转换为整数），并添加到总分子量中。
						bracket_mass_counter = 0;  // 重置 bracket_mass_counter，为下一个括号内的部分做准备。
                    }
                    continue;
				}

                // 如果字符既不是大写字母、小写字母，也不是数字，那么这就是一个无效的化学式，因为化学式只能包含这些类型的字符。
                else {
                    cout << "\033[31m[-]\033[0m Invalid character '" << chemicals[i] << "' in chemical formula." << endl;
                    break;
                }
            }
			// 验证完成，输出分子量结果。当然，先需要移除掉之前添加的特殊字符 "*"，以便在输出结果时显示正确的化学式。然后根据 success_signal 的值来判断是否成功计算分子量，如果成功则输出结果，否则输出错误信息。
            if (success_signal) {
				chemicals = chemicals.substr(0, chemicals.length() - 5);  // 移除之前添加的特殊字符 "*"，以便在输出结果时显示正确的化学式
                cout << endl << "\033[32m[+]\033[0m Finished calculate successfully!" << endl;
                cout << "\033[32m[+]\033[0m";
                for (int i = 0; i < 50; i++) cout << "=";
                cout << endl << "\033[32m[+]\033[0m Molecular mass of \033[32m" << chemicals.substr(0, chemicals.length() - 1) << "\033[0m is: \033[32m" << total_mass << "\033[0m" << endl;
                cout << "\033[32m[+]\033[0m";
                for (int i = 0; i < 50; i++) cout << "=";
                cout << endl << endl;
            }
            else {
				cout << "\033[31m[-]\033[0m Failed to calculate molecular mass due to invalid chemical formula." << endl;
            }
        }
    }
    return 0;
}
