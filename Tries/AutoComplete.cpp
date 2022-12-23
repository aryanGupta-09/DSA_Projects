#include<bits/stdc++.h>
using namespace std;

typedef struct Node{
    Node* next[26];
    int cnt;
    bool isEnding;

    Node(){
        cnt=0;
        isEnding=false;
        for(int i=0; i<26; i++) next[i]=NULL;
    }
}Node;

void insertString(Node *curr, string &s, int index){
    curr->cnt+=1;
    if(index==s.length()){
        curr->isEnding=true;
        return;
    }
    int nextIndex = (int)(s[index]-'a');
    if(curr->next[nextIndex]==NULL){
        curr->next[nextIndex] = new Node();
    }
    insertString(curr->next[nextIndex], s, index+1);
}

void addAllStrings(Node *curr, vector<string> &ans, string currStr){
    if(curr->isEnding){
        ans.push_back(currStr);
    }
    for(int i=0; i<26; i++){
        if(curr->next[i]!=NULL){
            addAllStrings(curr->next[i], ans, currStr+(char)('a'+i));
        }
    }
}

void searchString(Node *curr, string &s, int index, vector<string> &ans, string currStr){
    if(index==s.length()){
        addAllStrings(curr, ans, currStr);
        return;
    }
    int nextIndex = (int)(s[index]-'a');
    if(curr->next[nextIndex]==NULL){
        return;
    }
    searchString(curr->next[nextIndex], s, index+1, ans, currStr+s[index]);
}

vector<string> spellChecker(Node* root, string &query){
    vector<string> ans;
    searchString(root, query, 0, ans, "");
    return ans;
}

int main(){
    int n;
    cin>>n;
    Node* root = new Node();
    for(int i=0; i<n; i++){
        string s;
        cin>>s;
        insertString(root, s, 0);
    }
    int q;
    cin>>q;
    for(int i=0; i<q; i++){
        string query;
        cin>>query;
        vector<string> smallAns = spellChecker(root, query);
        if(smallAns.size()==0){
            insertString(root, query, 0);
            cout<<"No suggestions\n";
        }else{
            for(string s: smallAns) cout<<s<<endl;
        }
    }
}
