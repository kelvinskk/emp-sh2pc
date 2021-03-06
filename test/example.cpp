#include "emp-sh2pc/emp-sh2pc.h"
#include "integer.h"
#include "batcher.h"
#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/sm2_params.h"
#endif//

using namespace emp;
using namespace std;


void test_millionare(int party, int number) {
	Integer a(32, number, ALICE);
	Integer b(32, number, BOB);

	cout << "ALICE Input:\t"<<a.reveal()<<endl;
	cout << "BOB Input:\t"<<b.reveal()<<endl;
	cout << "ALICE larger?\t"<< (a>b).reveal()<<endl;
}

void test_sort(int party) {
	int size = 10;
	Batcher batcher1, batcher2;
	Integer *A = new Integer[size];
	for(int i = 0; i < size; ++i) {
		batcher1.add<Integer>(32, rand()%1024);
		batcher2.add<Integer>(32, rand()%1024);
	}

	batcher1.make_semi_honest(ALICE);
	batcher2.make_semi_honest(BOB);

	for(int i = 0; i < size; ++i)
		A[i] = batcher1.next<Integer>() ^ batcher2.next<Integer>();

	sort(A, size);
	for(int i = 0; i < size; ++i)
		cout << A[i].reveal_string()<<endl;
}

int main(int argc, char** argv) {
	int port, party;
	parse_party_and_port(argv, &party, &port);

#ifdef OT_NP_USE_MIRACL
	SM2_Init();
#endif//
	NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);

	setup_semi_honest(io, party);
	test_millionare(party, atoi(argv[3]));
	test_sort(party);
	delete io;
}
