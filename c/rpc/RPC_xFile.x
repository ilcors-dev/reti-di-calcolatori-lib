const ARR_MAX_LEN = 25;
const TESTS_LEN = 10;

struct TestStruct {
		string nome<>;
		int arr<ARR_MAX_LEN>;
	};

struct Risultato {
		TestStruct tests[TESTS_LEN];
		/* 0 = ok, -1 = errore */
		int res;
	};

program SERVPROG {
	version SERVVERS {  
		/* 0 = ok, -1 = errore */
		Risultato test(int) = 1;
	} = 1;
} = 0x20000020;
