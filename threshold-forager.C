// Same as sim5.C (with nearest neighbor jumps instead of corners (actual random walk)) for threshold>0 for 2D
#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;
const int D=2;

class BST
	{
	private:
	struct tree_node
		{
		tree_node* left;
		tree_node* right;
		vector<int> data;
	};
    
	public:
	tree_node* root;
	BST()
		{
		root = NULL;
	        }
       
        bool add(vector<int>);
        void destroy(tree_node*);
	};

int main()
	{
	const int configurations=100, Smax=1048577;
	int seed=1, arrayid=2;
//	cin >> seed >> arrayid;
	
	mt19937 gen(seed);
	uniform_int_distribution<int> binrand(0,1);
	
	int S=24, threshold=1;

	string outputfilename = "test-threshold-2D-";
	outputfilename += "k"; 
	outputfilename += to_string(threshold);
	outputfilename += "-";
	outputfilename += to_string(arrayid); 
	
	ofstream output; output.open(outputfilename.c_str());
	

	for(S=2; S<Smax; S+=S)
//	for(threshold=0; threshold<S; threshold++)
		{
		binomial_distribution<int> binom(S-threshold,0.5L);
		bernoulli_distribution bernoulli(0.5L);
		
   		double mavg=0.L, m2avg=0.L, tavg=0.L, t2avg=0.L;
		for(int config=0; config<configurations; config++)
			{
			BST emptied;	
			long int t=0, m=0;
			vector <int> x; x.resize(D);
			for(int d=0; d<D; d++) x[d]=0;			
			emptied.add(x);
			int s=S;
			
			while(s>0)
				{
				//for(int d=0; d<D; d++) x[d] += (2*binom(gen)-S);
				int nx=binom(gen), ny=S-threshold-nx;
				binomial_distribution<int> binom0(nx,0.5L); binomial_distribution<int> binom1(ny,0.5L);
				x[0] += 2*binom0(gen)-nx; x[1] += 2*binom1(gen)-ny;
				t += S-threshold;
				s = threshold;
				if(emptied.add(x))
					{
					s = S;
					m++;
					}
				else
					{
					while(s>0)
						{
						int xory=bernoulli(gen);
						if(xory==0) x[0] += 2*bernoulli(gen)-1;
						else x[1] += 2*bernoulli(gen)-1;
						t++;
						if(emptied.add(x))
							{
							s = S;
							m++;
							break;
							}
						else s--;
						}
					}
				}
			mavg += (double)m; m2avg += (double)m*(double)m;
			tavg += (double)t; t2avg += (double)t*(double)t;
			emptied.destroy(emptied.root);
			output << m << '\t' << t << endl;
			}
//		cout << S << '\t' << threshold << '\t' << mavg/(double)configurations << '\t' << sqrt(m2avg/(double)configurations - (mavg/(double)configurations)*(mavg/(double)configurations))/sqrt(configurations) << '\t' << tavg/(double)configurations << '\t' << sqrt(t2avg/(double)configurations - (tavg/(double)configurations)*(tavg/(double)configurations))/sqrt(configurations) << '\t' << configurations << endl;	
		}	
	
	output.close();
	}
	
bool BST::add(vector<int> d)
	{
	tree_node* t = new tree_node;
	tree_node* parent;
	t->data = d;
	t->left = NULL;
	t->right = NULL;
	parent = NULL;
    
	// is this a new tree?
	if(root==NULL){ root = t; return 1; }
	else
		{
		//Note: ALL additions are as leaf nodes
		tree_node* curr;
		curr = root;
		// Find the Node's parent
		while(curr)
			{
			parent = curr;
			if(t->data > curr->data) curr = curr->right;
			else if(t->data < curr->data) curr = curr->left;
			else return 0;
			}
		if(t->data < parent->data){ parent->left = t; return 1; }
		else if(t->data > parent->data){ parent->right = t; return 1; }
		else return 0;
		}
	}
        
void BST::destroy(tree_node* root)
	{ 
	if(root)
		{
		destroy(root->left);
		destroy(root->right);
		delete(root);
		}
	}
        

