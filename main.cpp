#include <iostream>
#include <list>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;
// –input <FILE>
char* fileName;
bool inputExists = false;
// –solseq
bool solseq = false;
// -pcost
bool pcost = false;
int numPcost = 0;
// -nvisited
bool visited = false;
int numVisited = 0;
// -h
bool h = false;
int H = 1;
// -rand

int N=3;
int M=2;
int ** start;
int ** goal;
bool SolutionExists = false;
/*int UP[2] = {-1,0};
int RIGHT[2] = {0,1};
int DOWN[2] = {1,0};
int LEFT[2] = {0,-1};*/
typedef struct Node
{
    double g;
    double f;
    double h;
    int** a;
    struct Node* parent;
} Node;
list<Node> open;
list<Node> closed;
void ReadFile();
void ReadStandardInput();
void Print(int**);
void GenerateGoal();
bool IsSimilar(int**,int**);
void GenerateRand();
Node getNodeOpenLowestF();
void aStar();
int Heuristic_1(int**);
void remove(list<Node>&,Node*);
double Heuristic_2(int**);
Node find(list<Node>&,int**);
list <Node> getSuccessors(Node*);


int main(int argc, const char** argv)
{
    if(argc > 1)
    {
        for(int i=0; i<argc; ++i)
        {
            if(strcmp(argv[i], "-input") == 0)
            {
                inputExists==true;
                sscanf(argv[i+1], "%s",&fileName);
                printf("FileName: %s", fileName);
            }
            if(strcmp(argv[i], "-solseq") == 0)
            {
                solseq == true;
            }
            if(strcmp(argv[i], "-pcost") == 0)
            {
                pcost == true;
            }
            if(strcmp(argv[i], "-nvisited") == 0)
            {
                visited == true;
            }
            if(strcmp(argv[i], "-h") == 0)
            {
                h = true;
                sscanf(argv[i+1], "%i", &H);
            }
            if(strcmp(argv[i], "-rand") == 0)
            {
                sscanf(argv[i+1], "%i", &N);
                sscanf(argv[i+2], "%i", &M);
                GenerateRand();
            }
        }
    }
    if(inputExists==true)
    {
        ReadFile();
    }
    else
    {
        ReadStandardInput();
    }
    //Print(start);
    GenerateGoal();
    //Print(goal);
    aStar();



    return 0;
}
void ReadFile(){
    ifstream fin;
    fin.open(fileName);
    fin >> N;
    start = new int* [N];
    for(int i = 0; i < N; ++i)
    {
        start[i] = new int [N];
    }
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            fin >> start[i][j];
        }
    }
    fin.close();
}
void ReadStandardInput(){
    cout << "N= ";
    cin >> N;
    start = new int* [N];
    for(int i = 0; i < N; ++i)
    {
        start[i] = new int [N];
    }
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            cout << "start[" << i << "][" << j << "]= ";
            cin >> start[i][j];
        }
    }
}
void Print(int**a){
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            cout << setw(4) << a[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void GenerateGoal(){
    goal = new int* [N];
    for(int i = 0; i < N; ++i)
    {
        goal[i] = new int [N];
    }
    int counter = 0;
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            goal[i][j] = counter;
            counter++;
        }
    }
}
bool IsSimilar(int** a, int** b){
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            if(a[i][j] != b[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
void GenerateRand(){
    start = new int* [N];
    for(int i = 0; i < N; ++i)
    {
        start[i] = new int [N];
    }
    GenerateGoal();
    start=goal;
    Print(start);
    srand(time(NULL));
    int x = 0;
    int y = 0;
    for(int i = 0; i < M; ++i)
    {
        //Print(start);
        int step = rand() % 4;
        switch(step)
        {
        case 0: //UP [-1,0]
            if((x - 1) >= 0)
            {
                start[x][y] = start[x-1][y];
                start[x-1][y] = 0;
                x--;
            }
            else
                i--;
        case 1: //RIGHT [0,1]
            if((y + 1) < N)
            {
                start[x][y] = start[x][y+1];
                start[x][y+1] = 0;
                y++;
            }
            else
                i--;
        case 2: //DOWN [1,0]
            if((x + 1) < N)
            {
                start[x][y] = start[x+1][y];
                start[x+1][y] = 0;
                x++;
            }
            else
                i--;
        case 3: //LEFT [0,-1]
            if((y - 1) >= 0)
            {
                start[x][y] = start[x][y-1];
                start[x][y-1] = 0;
                y--;
            }
            else
                i--;
        }
    }
    Print(start);
}
Node getNodeOpenLowestF(){
    Node min = open.front();
    int fmin = min.f;
    for(auto& i:open)
    {
        if(i.f < fmin)
        {
            fmin = i.f;
            min = i;
        }
    }
    return min;
}
void aStar()
{
    Node startN;
    startN.a = start;
    startN.g = 0;
    if(H == 1)
    {
        startN.h = Heuristic_1(start);
    }
    if(H == 2)
    {
        startN.h = Heuristic_2(start);
    }

    startN.f = startN.g + startN.h;
    startN.parent = NULL;
    open.push_front(startN);
    Node node;
    while(open.size())
    {
        numVisited++;
        node = getNodeOpenLowestF();
        closed.push_front(node);
        if(IsSimilar(node.a,goal))
        {
            SolutionExists = true;
            break;
        }

        list<Node>successors = getSuccessors(&node);
        for(auto &i :successors )
        {
            Node x = find(open,i.a);
            if(x.h >= 0)
            {
                if(x.f <= i.f)
                {
                    continue;
                }
                else
                {
                   remove(open,&x);
                }
            }
            x = find(closed, i.a);
            if(x.h >= 0)
            {
                if(x.f <= i.f)
                {
                    continue;
                }
                else
                {
                    remove(closed,&x);
                }
            }
            open.push_front(i);
        }
    }

    if(SolutionExists)
    {
        if(visited)
        {
            cout << "Number of visited nodes: " << numVisited << endl;
        }


    }

}
int Heuristic_1(int** a){
    int counter=0;
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            if((a[i][j] != N * i + j) && a[i][j] != 0)
            {
                counter++;
            }
        }
    }
    return counter;
}
double Heuristic_2(int** a){
    int r,c;
    double dis = 0.0;
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            r = a[i][j] / N;
            c = a[i][j] % N;
            dis += abs(i - r) + abs(j-c);
        }
    }
    return dis;
}
void remove(list <Node>& l,Node* x){
    for (list<Node>::const_iterator itr = l.cbegin(); itr != l.end(); itr++)
	{
		if (itr->a == x->a)
		{
			l.erase(itr--);
			break;
		}
	}
}
Node find(list<Node>& l,int **m) {
    Node x;
	x.h = -1;
	for (auto& i : l)
	{		if (IsSimilar(m, i.a)) {
			return i;
		}
	}
	return x;
}
list <Node> getSuccessors(Node* par){
    list <Node> l;
    int x,y;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; ++j)
        {
            if (par->a == 0)
            {
                x = i;
                y = j;
                break;
            }
        }
    }

    if((x - 1) >= 0)
    {
        Node node;
        node.a=par->a;
        node.a[x][y] = node.a[x-1][y];
        node.a[x-1][y] = 0;
        node.g = par->g + 1;
        node.parent = par;
        if(H == 1)
        {
            node.h = Heuristic_1(node.a);
        }
        if(H == 2)
        {
            node.h = Heuristic_2(node.a);
        }
        l.push_front(node);
    }
    if((y + 1) < N)
    {
        Node node;
        node.a=par->a;
        node.a[x][y] = node.a[x][y+1];
        node.a[x][y+1] = 0;
        node.g = par->g + 1;
        node.parent = par;
        if(H == 1)
        {
            node.h = Heuristic_1(node.a);
        }
        if(H == 2)
        {
            node.h = Heuristic_2(node.a);
        }
        l.push_front(node);
    }
    if((x + 1) < N)
    {
        Node node;
        node.a=par->a;
        node.a[x][y] = node.a[x+1][y];
        node.a[x+1][y] = 0;
        node.g = par->g + 1;
        node.parent = par;
        if(H == 1)
        {
            node.h = Heuristic_1(node.a);
        }
        if(H == 2)
        {
            node.h = Heuristic_2(node.a);
        }
        l.push_front(node);
    }
    if((y - 1) >= 0)
    {
        Node node;
        node.a=par->a;
        node.a[x][y] = node.a[x][y-1];
        node.a[x][y-1] = 0;
        node.g = par->g + 1;
        node.parent = par;
        if(H == 1)
        {
            node.h = Heuristic_1(node.a);
        }
        if(H == 2)
        {
            node.h = Heuristic_2(node.a);
        }
        l.push_front(node);
    }
    return l;
}

