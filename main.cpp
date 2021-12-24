#include<iostream>
#include<math.h>
#include<map>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include<fstream>

using namespace std;

class node                  // The node structure in Fibonacci heap; description of its attributes.
{
    public:
    int occurence;
    string hash_tag;
    unsigned long int degree;
    bool child_cut;
    node* left_sib;
    node* right_sib;
    node* child;
    node* parent;

//constructor that initializes a new node in the fibonacci heap
    node(int value, string tag)
    {
        this->left_sib=this;
        this->right_sib=this;
        this->parent=NULL;
        this->child=NULL;
        this->child_cut=false;
        this->degree=0;
        this->occurence=value;
        this->hash_tag=tag;
    }

};


map< string, node*> locationhash;
map< string, node*>::iterator it;
node* root_node=NULL;
static unsigned long int No_nodes;

// the function inserts a new hashtag in the fibonacci max heap
//if the heap is empty then the new node is made the root_node
// else then insert the new node in the root_node level sibling list
void insert_node_function(node* node_to_insert, bool old)
{
    if(root_node==NULL)
    {
        root_node=node_to_insert;
        root_node->parent=NULL;
        if(old!=true)
        {
            No_nodes++;
        }
    }
    else
    {
        node_to_insert->right_sib=root_node;
        root_node->left_sib->right_sib=node_to_insert;
        node_to_insert->left_sib=root_node->left_sib;
        root_node->left_sib=node_to_insert;
        node_to_insert->parent=NULL;
        if(old!=true)
        {
            No_nodes++;
        }

        if(node_to_insert->occurence>root_node->occurence)
        {
            root_node=node_to_insert;
        }
    }
}



//the function performs merging of two sub trees
//the one with the larger occurence will become the parent and the other one the child
void combine_two_trees_function(node* c,node* p)
{
    (c->left_sib)->right_sib=c->right_sib;
        (c->right_sib)->left_sib=c->left_sib;
    c->parent=p;
    c->child_cut=false;
    if(p->child==NULL)
    {
        p->child=c;
        c->left_sib=c;
        c->right_sib=c;
    }
    else
    {
        c->right_sib=p->child;
        c->left_sib=(p->child)->left_sib;
        ((p->child)->left_sib)->right_sib=c;
        (p->child)->left_sib=c;
        if(c->occurence<(p->child)->occurence)
        {
            p->child=c;
        }
    }
    p->degree++;
}



    //this function will merge two subtrees that have the same degree until there s only one tree left
void pairwise_combine()
{
    unsigned long int maxdegree=No_nodes;
    vector<node*> table;
    unsigned long int toplevel_nodes=0;
    if(root_node==NULL)
    {
        return;
    }
    else
    {
        node* temp_ptr=root_node;
        do
        {
            ++toplevel_nodes;
            temp_ptr=temp_ptr->right_sib;
        }while(temp_ptr!=root_node);
    }

    node* current=root_node;
    node* combine_child=NULL;
    node* combine_as_parent=NULL;
    node* nextnode=NULL;
    unsigned long int node_degree=0;
    while(toplevel_nodes>0)
    {
        nextnode=current->right_sib;
        node_degree=current->degree;
        while(node_degree>=table.size())
        {
            table.push_back(NULL);
        }
        while(table[node_degree]!=NULL && table[node_degree]!=current)
            {
            if(table[node_degree]->occurence>current->occurence)
            {
                combine_as_parent=table[node_degree];
                combine_child=current;
                combine_two_trees_function(combine_child,combine_as_parent);     //combine the two trees
                if(combine_child==root_node)
                {
                    root_node=combine_as_parent;
                }
                current=combine_as_parent;
            }
            else
            {                                                               //vice versa of the above
                combine_as_parent=current;
                combine_child=table[node_degree];
                combine_two_trees_function(combine_child,combine_as_parent);
                if(combine_child==root_node)
                {
                    root_node=combine_as_parent;
                }
                current=combine_as_parent;
            }
            table[node_degree]=NULL;
            ++node_degree;
            while(node_degree>=table.size())
            {
                table.push_back(NULL);
            }
        }
        table[node_degree]=current;
        if(root_node->occurence<current->occurence)
        {
            root_node=current;
        }
        current=nextnode;                                                  //scan next node in the lisr
        --toplevel_nodes;
    }


}

//  Remove the node with the maximum occurence from the heap
// its subtress are then re-inerted into the heap at the root_node level
// then a pairwise combining function is called

node* removemax_function()
{
    node* removed=root_node;

    if(root_node==NULL)
    {
        return root_node;
    }
    else
    {
        if(removed->child!=NULL)
        {
            node* childnode=removed->child;
            node* nextnode;
            node* firstchild=childnode;
            do
            {
                nextnode=childnode->right_sib;
                childnode->left_sib=childnode;
                childnode->right_sib=childnode;
                insert_node_function(childnode,true);
                childnode=nextnode;
            }while(nextnode!=firstchild);
        }

        if(removed==removed->right_sib && removed->child==NULL)
        {
            root_node=NULL;
        }
        else
        {
            removed->right_sib->left_sib=removed->left_sib;
            removed->left_sib->right_sib=removed->right_sib;
            root_node=removed->right_sib;
            pairwise_combine();
        }
        removed->left_sib=removed;
        removed->right_sib=removed;
        removed->parent=NULL;
        removed->child=NULL;
        removed->child_cut=false;
        removed->degree=0;
        /*No_nodes--;                                           Not decreasing number of nodes as number of nodes will not exactly decrease as they will be re-inserted and also maximum number of extract max calling can be 20 at once so,
                                                                incomparsion to the number of nodes in the heap(nearly million) this is very small.*/
        return removed;                                         //return the maximum occurence hashtag node
    }
}

// cut a given node from its parent's child list
// reduc ethe degree of the parent since it is losing a child

void cut_function(node* cutnode, node* parent_node)
{
    if(cutnode->right_sib!=cutnode)             // remove node from child list of parent
    {
        cutnode->left_sib->right_sib=cutnode->right_sib;
        cutnode->right_sib->left_sib=cutnode->left_sib;
    }

    --parent_node->degree;                  //reduce degree of parent

    if(cutnode==parent_node->child)         //If the parent's child pointer is set to this node
    {
        if(cutnode->right_sib!=cutnode)         //If it has mre children change the parent's child pointer to next node in the childlist
        {
            parent_node->child=cutnode->right_sib;
        }
        else
        {
            parent_node->child=NULL;        //if this node ws the only child then the parent does not have any child after the cut
        }
    }
    cutnode->left_sib=cutnode;
    cutnode->right_sib=cutnode;
    cutnode->child_cut=false;
    insert_node_function(cutnode,true);
}


    // this function performs cascading cut on the parent if the childcut value of the parent is true.
    // the parent is also detached from its parent until the parent is root_node or the childcut value of the parent is false

void cascade_cut_function(node* affected)
{
    node* parent_of_affected=affected->parent;
    if(parent_of_affected!=NULL)                    //Cascade cut only if there is a parent
    {
        if(affected->child_cut==false)               //If childcut of parent is false, set it to true and return
        {
            affected->child_cut=true;
        }
        else                                        //else cut this subtree root_nodeed at this node and cascade operation upwards to its parent
        {
            cut_function(affected,parent_of_affected);
            cascade_cut_function(parent_of_affected);
        }
    }
}


// this function increase the occurence of the node by the given value.
// after increasing the occurence frequency the frequency of the node may get larger than its parent

void increase_occurence(node* changenode)
{
    node* affected_parent=changenode->parent;
    if(affected_parent!=NULL && changenode->occurence>affected_parent->occurence) //A cut operation is done if the frequency of the node is greater than the frequency of its parent
    {
        cut_function(changenode,affected_parent);
        cascade_cut_function(affected_parent);
    }
    if(changenode->occurence>root_node->occurence)
    {
        root_node=changenode;
    }
    return;
}



int main(int argc, char *argv[])
{
    if(argc!=2)                                                         //check if filename was passed as argumenr
    {
        cout<<"No input file name found";
        return 0;
    }
    else
    {
        ofstream    pout;
        ifstream    pin;
        pin.open(argv[1]);
        pout.open("output_file.txt");

        string hash_tag,outputhash_tag;
        unsigned long int freq=0,remove_n_maxs=0,iter=0;
        while(pin>>hash_tag)
        {
            if(hash_tag[0]=='s' || hash_tag[0]=='S')
            {
                fclose(stdin);
                fclose(stdout);
                return 0;
            }
            else if(hash_tag[0]=='#')
            {
                pin>>freq;
                //hash_tag=hash_tag.erase(0,1);
                it=locationhash.find(hash_tag);
                if(it!=locationhash.end())
                {
                    node* incrementnode=it->second;
                    incrementnode->occurence+=freq;
                    increase_occurence(incrementnode);
                }
                else
                {
                    node* new_hash_tag = new node(freq, hash_tag);
                    locationhash[hash_tag]=new_hash_tag;
                    insert_node_function(new_hash_tag,false);
                }
            }
            else if(hash_tag[0]!='#')                                    //if the inpu does not match either of the coditions, it will the count of number of remove maxs
            {
                remove_n_maxs= atoi(hash_tag.c_str());
                vector<node* > removed_nodes;
                iter=0;
                while(iter<remove_n_maxs)                          //remove n tp occurence noes from the heap
                {
                    node* maxhash=removemax_function();
                    removed_nodes.push_back(maxhash);
                    outputhash_tag=maxhash->hash_tag;                   //store the removed nodes i a vector
                    outputhash_tag=outputhash_tag.erase(0,1);
					pout<<outputhash_tag;//maxhash->hashtag;
					if((iter+1)!=remove_n_maxs)
					pout<<",";
                    ++iter;
                }
				pout<<endl;
                iter=0;
                while(iter<remove_n_maxs)                          //iteratively re-insert the removed nodes into the heap
                {
                  insert_node_function(removed_nodes[iter],false);
                  ++iter;
                }
            }
            hash_tag="";
        }
    }
    return 0;
}
