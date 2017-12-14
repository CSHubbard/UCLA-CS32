#include "Sequence.h"

int main()
{

	Sequence n,p,q;			//initializes three seq
	//Sequence  p;

	interleave(n, p, q);	//tests with empty sequences

	n.insert(0, 30);		//populates seq n
	n.insert(1, 21);
	n.insert(2, 63);
	n.insert(3, 42);
	n.insert(4, 17);
	n.insert(5, 63);

	interleave(n, p, q);	//test with an empty seq

	////tests beyond this point test for q being nonempty

	p.insert(0, 42);		//populates seq p
	p.insert(1, 63);
	p.insert(2, 84);
	p.insert(3, 19);

//	interleave(n, p, q);	//tests exmp in proj2 spec
////	interleave(p, n, q);	//test if first seq has less than second
//
//	//interleave(n, p, p);	//tests alaiasing of ending seq
//	//

}