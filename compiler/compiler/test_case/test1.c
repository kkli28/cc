
source = "char else enum if int return sizeof while";
int type = CHAR;
while (type <= WHILE) {
	next();
	auto& tk = table->getCurrentToken();
	tk.type = type;
	tk.setScope({ KEY_WORD_SCOPE });
	++type;
}

source = "printf malloc exit scanf getchar putchar";
type = I_PRTF;
while (type <= I_PUTC) {
	next();
	Token& tk = table->getCurrentToken();
	tk.klass = SYS_FUNC;
	tk.dataType = INT_TYPE;
	tk.value = type++;
	tk.setScope({ KEY_WORD_SCOPE });
}

source = "void";
next(FORMAT(format));
Token& tk = table->getCurrentToken(FORMAT(format));
tk.type = CHAR;
tk.setScope({ KEY_WORD_SCOPE });

source = "main";
next();
table->setMainToken();
table->getCurrentToken().setScope({ KEY_WORD_SCOPE });

