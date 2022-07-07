#include <bits/stdc++.h>
using namespace std;

struct Node {	
	float data;
	int x, y;
	Node* child[2];
	Node() {
		for(int i = 0; i < 2; i++)
			child[i] = NULL;
	}
};

bool mark[1001000];
float value[1001000];

Node *T1, *T2;

void Insert(Node* &T, int id, float data) {
	// cout << id << " " << data << endl;
	if(!mark[id])
		return;
	if(T == NULL) {
		T = new Node;
		T ->data = data;
		T ->child[0] = T ->child[1] = NULL;
	}
	for(int i = 0; i < 2; i++)
		Insert(T ->child[i], 2 * id + i, value[2 * id + i]);
}

bool Search(Node* T, float val) {
	if(T == NULL)
		return false;
	if(T ->data == val)
		return true;
	return Search(T ->child[0], val) || Search(T ->child[1], val);
}

// dir = 0: chen ben trai, dir = 1: chen ben phai
void InsertNode(Node* &T, float at, int dir, float val) {
	if(T == NULL)
		return;
	// cout << T ->data << endl;
	if(T ->data == at) {
		// if(T ->child[dir] != NULL)
		// 	return;
		T ->child[dir] = new Node;
		T ->child[dir] ->data = val;
		T ->child[dir] ->child[0] = T ->child[dir] ->child[1] = NULL;
		printf("Da chen!\n");
		return;
	}
	for(int i = 0; i < 2; i++)
		InsertNode(T ->child[i], at, dir, val);
}

void DeleteNode(Node* &T, float val) {
	if(T == NULL)
		return;
	if(T ->data == val) {
		if(T ->child[0] != NULL || T ->child[1] != NULL)
			return;
		T = NULL;
		return;
	}
	for(int i = 0; i < 2; i++)
		DeleteNode(T ->child[i], val);
}

void DeleteTree(Node* &T) {
	T = NULL;
}

void Copy(Node* &T1, Node* T, int &r, int &c, int k) {
	if(T != NULL) {
		T1 = new Node;
		T1 ->data = T ->data;
		r += k;
		Copy(T1 ->child[0], T ->child[0], r, c, k);
		T1 ->x = c;
		T1 ->y = r;
		c += k;
		Copy(T1 ->child[1], T ->child[1], r, c, k);
		r -= k;
	}
	else T1 = NULL;
}

int maxPrintedX = 30;

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x); 
    maxPrintedX = max(maxPrintedX, y);
}

//ham Line dung de ve duong noi giua 2 diem
void Line(int column1, int row1, int column2, int row2, int k) {
	int n; 
	n = (column2 - column1) / k;
	for(int i=1;i<=k;i++) {
		gotoxy(column1 + i * n, row1 + i);
		printf("*");
	}
}
/*------------------------*/
/* Duyet theo goc-trai-phai. Chuong trinh kiem tra node con ben trai,
neu ton tai thi ve duong dan den node do va in gia tri cua node con trai do*/
void Display(Node* T, int k) {
	if(T == NULL)
		return;
	Node* p;
	gotoxy(T ->x, T ->y);
	printf("%g", T ->data);
	for(int i = 0; i < 2; i++)
		if(T ->child[i] != NULL) {
			p = T ->child[i];
			Line(T ->x, T ->y, p ->x, p ->y, k);
			Display(T ->child[i], k);
		}
}

void printTree(Node* T, int x = 20, int y = 40, int k = 2) {
	Node* T1;
	Copy(T1, T, x, y, k);
	if(T1 != NULL) 
		Display(T1, k);
	DeleteTree(T1);
}

// Neu 1 trong 2 Node = NULL thi tra ve true neu ca 2 cung bang NULL, nguoc lai tra ve false
// Neu ca 2 Node != NULL, kiem tra xem Node T1 va T2 co cung hinh dang hay khong bang cach kiem ra su ton tai cua 2 Node con cua moi nut. Sau do de quy kiem tra cac Node con cua T1, T2.
bool isSimilar(Node* T1, Node* T2) {
	if(T1 == NULL || T2 == NULL)
		return T1 == NULL && T2 == NULL;

	bool res = !((T1 ->child[0] == NULL) ^ (T2 ->child[0] == NULL)) && !((T1 ->child[1] == NULL) ^ (T2 ->child[1] == NULL));

	for(int i = 0; i < 2; i++)
		res = res && isSimilar(T1 ->child[i], T2 ->child[i]);

	return res;
}

// Doc du lieu cua cay tu ban phim, tra ve true neu doc thanh cong, nguoc lai tre ve false
bool ReadTree_User(Node* &T) {
	set<float> S;
	fflush(stdin);
	int n;
	printf("Nhap so tang ");
	if(!scanf("%d", &n))
		return false;
	if(n <= 0)
		return true;
	printf("Nhap du lieu cay ( 0 : rong ; 1 : ton tai )");
	for(int i = 1; i < pow(2, n); i++)
		if(!scanf("%d", &mark[i]))
			return false;
	printf("Nhap phan tu ( 0 duoc trung ): ");
	for(int i = 1; i < pow(2, n); i++)
		if(!scanf("%f", &value[i]))
			return false;
	for(int i = 1; i < pow(2, n); i++) {
		if(!mark[i])
			continue;
		if(S.count(value[i]))
			return false;
		S.insert(value[i]);
	}
	Insert(T, 1, value[1]);
	return true;
}

// Doc du lieu cua cay tu file, tra ve true neu doc thanh cong, nguoc lai tre ve false
bool ReadTree_File(Node* &T) {
	set<float> S;
	fflush(stdin);
	string PATH;
	printf("Duong dan toi file: ");
	if(!getline(cin, PATH))
		return false;
	FILE *f;
	if(!(f = fopen(PATH.c_str(), "r"))) {
		printf("File khong ton tai!\n");
		return false;
	}

	int n;
	if(!fscanf(f, "%d", &n))
		return false;
	if(n <= 0)
		return true;
	for(int i = 1; i < pow(2, n); i++)
		if(!fscanf(f, "%d", &mark[i]))
			return false;
	for(int i = 1; i < pow(2, n); i++)
		if(!fscanf(f, "%f", &value[i]))
			return false;
	for(int i = 1; i < pow(2, n); i++) {
		if(!mark[i])
			continue;
		if(S.count(value[i]))
			return false;
		S.insert(value[i]);
	}
	Insert(T, 1, value[1]);

	// for(int i = 1; i < pow(2, n); i++)
	// 	cout << mark[i] << " " << value[i] << endl;
	// system("pause");
	return true;
}

bool ReadTree(Node* &T, string MODE = "USER") {
	if(MODE == "USER")
		return ReadTree_User(T);
	if(MODE == "FILE")
		return ReadTree_File(T);
	return false;
}

bool InsertNodeToTree() {

}

bool DeleteNodeFromTree() {

}

// In ra man chinh tu cac file
void PrintScene(string SCENE) {
	ifstream inp(SCENE.c_str());

	string s;
	while (getline(inp, s))
		cout << s << endl;
}

// Xoa man hinh
void ClearScreen() {
	system("cls");
}

// Doc lua chon tu ban phim
int GetChoice(string prefix = "") {
	fflush(stdin);
	cout << prefix << "Nhap lua chon cua ban: ";
	int c;
	if(!scanf("%d", &c))
		return -1;
	return c;
}

// Man hinh chinh
void MainMenu() {
	maxPrintedX = 30;
	ClearScreen();
	PrintScene("Display\\MainMenu.txt");
	int c = GetChoice("	 	");
	if(c == 0)
		exit(0);
	else if(c == 1) {
		DeleteTree(T1);
		DeleteTree(T2);
		ClearScreen();
		PrintScene("Display\\ReadChoice.txt");
		int mode = GetChoice("	 	");
		bool successful = false;
		if(mode == 0) {
			// ClearScreen();
			printf("Nhap cay 1:\n");
			successful |= ReadTree(T1);
			if(successful) {
				printf("Nhap cay 2:\n");
				successful |= ReadTree(T2);
			}
		} else if(mode == 1) {
			// ClearScreen();
			printf("Doc cay 1:\n");
			successful |= ReadTree(T1, "FILE");
			if(successful) {
				printf("Doc cay 2:\n");
				successful |= ReadTree(T2, "FILE");
			}
		} else {
			printf("Vui long nhap lai lua chon!\n");
			system("pause");
		}
		if(!successful) {
			DeleteTree(T1);
			DeleteTree(T2);
			printf("Doc cay khong thanh cong, vui long doc lai!\n");
			system("pause");
		}
	} else if(c == 2) {
		//ClearScreen();
		printf("\n\nCay 1:\n\n");
		printTree(T1, maxPrintedX, 40);
		gotoxy(0, maxPrintedX + 1);
		printf("\n\nCay 2:\n\n");
		printTree(T2, maxPrintedX + 2, 40);
		gotoxy(0, maxPrintedX + 1);
		system("pause");
	} else if(c == 3) {
		//ClearScreen();
		printf("\n\nCay 1:\n\n");
		printTree(T1, maxPrintedX, 40);
		gotoxy(0, maxPrintedX + 1);
		printf("\n\nCay 2:\n\n");
		printTree(T2, maxPrintedX + 2, 40);
		gotoxy(0, maxPrintedX + 1);
		printf("Chon cay nhi phan can chen (1: Cay 1, 2: Cay 2)\n");
		int c = GetChoice();
		if(c < 1 || c > 2) {
			printf("Vui long nhap lai lua chon!\n");
			system("pause");
			return;
		}
		Node **T;
		if(c == 1)
			T = &T1;
		else
			T = &T2;
		float at, val;
		int dir;
		printf("Nhap a d v (a: Node de chen | d: vi tri chen, d = 0: ben trai, d = 1: ben phai | v: gia tri chen)\nNhap: ");
		while(!scanf("%f %d %f", &at, &dir, &val) || Search(*T, val)) {
			printf("Nhap khong thanh cong, vui long nhap lai!\n");
			system("pause");
			printf("Nhap: ");
		}
		InsertNode(*T, at, dir, val);
		system("pause");
	} else if(c == 4) {
		// ClearScreen();
		printf("\n\nCay 1:\n\n");
		printTree(T1, maxPrintedX, 40);
		gotoxy(0, maxPrintedX + 1);
		printf("\n\nCay 2:\n\n");
		printTree(T2, maxPrintedX + 2, 40);
		gotoxy(0, maxPrintedX + 1);
		printf("Chon cay nhi phan can chen (1: Cay 1, 2: Cay 2)\n");
		int c = GetChoice();
		if(c < 1 || c > 2) {
			printf("Vui long nhap lai lua chon!\n");
			system("pause");
			return;
		}
		Node **T;
		if(c == 1)
			T = &T1;
		else
			T = &T2;
		float val;
		printf("Nhap Node can xoa: ");
		while(!scanf("%f", &val)) {
			printf("Nhap khong thanh cong, vui long nhap lai!\n");
			system("pause");
			printf("Nhap Node can xoa: ");
		}
		DeleteNode(*T, val);
	} else if(c == 5) {
		DeleteTree(T1);
		DeleteTree(T2);
		printf("Da huy 2 cay nhi phan \n "); 
		system("pause");
	} else if(c == 6) {
		system("cls");
		printf("\n\nCay 1:\n\n");
		printTree(T1, 20, 40);
		gotoxy(0, maxPrintedX + 1);
		printf("\n\nCay 2:\n\n");
		printTree(T2, maxPrintedX + 2, 40);
		gotoxy(0, maxPrintedX + 1);

		bool similar = isSimilar(T1, T2);
		if(similar)
			PrintScene("Display\\Similar.txt");
		else
			PrintScene("Display\\NotSimilar.txt");;
		system("pause");
	} else {
		printf("Vui long nhap lai lua chon!\n");
		system("pause");
	}
}

int main() {
	while(true)
		MainMenu();
}
