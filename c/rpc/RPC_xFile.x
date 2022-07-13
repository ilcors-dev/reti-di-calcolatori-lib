const TESTS_LEN = 5;
const OUTPUT_LEN = 10;
const STR_LEN = 256;

struct Input {
	string strType<STR_LEN>;
	int intType;
};

struct TestStruct {
	string id<>;
	string bookerId<>;
	string brand<>;
	string imagesFolder<>;
};

struct Output {
	TestStruct tests[OUTPUT_LEN];
};

program SERVPROG {
	version SERVVERS {  
		/* 0 = ok, -1 = errore */
		int with_input_struct(Input) = 1;
		Output with_output_struct(char) = 2;
	} = 1;
} = 0x20000020;
