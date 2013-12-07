#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>

using namespace std;

struct CrossSolver {
  vector< vector<int> > a[2]; 
  vector<int> s[2];
  int L[2];
  vector< vector<int> >bb;
  vector<int> bbflag;
  void SetFirst(int l);
  bool GetNext(int l);
  bool TestPosition(int l);
  bool TestLine(int l);
  void FixLastColumnGroup();
  void SolveCrosspoint();
  void noSolution();
  bool Solved;
  bool Solution;
};

CrossSolver ways[8];

bool asciiart = false;

void NoSolution() {
  cout << "No solution\n";
}

void CrossSolver::noSolution() {
  Solution=0;
  Solved=true;
  pthread_exit(0); 
}

void CrossSolver::SetFirst(int l) {
  if (Solved) pthread_exit(0);
  bbflag[l]=0;
  for (int i=0;i<L[1];i++) bb[l][i]=0;
  a[0][l][0]=0;
  int used=0;
  for (int i=2;i<a[0][l].size()-1;used++,i+=2) a[0][l][i]=1;
  if ((a[0][l][a[0][l].size()-1]=L[1]-used-s[0][l])<0) noSolution();
}

bool CrossSolver::GetNext(int l) {
  if (Solved) pthread_exit(0);
  vector<int>& h = a[0][l];
  int n=h.size()-1;
  if (h[0]==L[1]-s[0][l]-(n>2?n/2-1:0)) return 0;
  if (h[n]) {
    h[n]--;
    h[n-2]++;
  }
  else {
    for (int i=n-2;i>0;i-=2) {
      if (h[i]>1) {
        h[i]--;
        h[i-2]++;
        if (h[i]>1) {
          h[n]=h[i]-1;
          h[i]=1;
        }
        break;
      }
    }
  }
  return true;
}

bool CrossSolver::TestPosition(int l) {
  int c=0;
  vector<int>& h = a[0][l];
  bool b=0;
  if (bbflag[l]) {  
    for (int k=0;k<h.size();k++,b=!b) 
      for (int i=0;i<h[k];i++,c++) {
        if (b) {
          if (bb[l][c]==1) return 0;
        }
        else if (bb[l][c]==2) return 0;
      }
    b=0;
    c=0;
  }
  for (int k=0;k<h.size();k++,b=!b) {
    for (int i=0;i<h[k];i++,c++) {
      if (b) {
        if (bb[l][c]==3) continue;
      }
      else if (bb[l][c]==4) continue;
      int used=0;
      int cl=0;
      vector<int>& v = a[1][c];
      bool d=0;
      bool found=0;
      for (int m=0;m<v.size();m++,d=!d) {
        for (int j=0;j<v[m];j++,cl++) {
          if (cl==l) {
            if (d^b) {
              if (d) {
                if (j||s[1][c]-used+(v.size()-m)/2+l>L[0]) { bbflag[l] = 1; bb[l][c] = 2; return 0; }
                v[m-1]++;
                bb[l][c]=4;
              }
              else {
                if (m==v.size()-1||m&&!j) { bbflag[l] = 1; bb[l][c] = 1; return 0; }
                v[m] = j;
                bb[l][c]=3;
              }
            } else { 
              if (!d) { v[m]=j+1; bb[l][c]=4; }
              else bb[l][c]=3;
            }
            found=true;
            break;
          }
          if (d) used++;
        }
        if (found) break;
      }
      if (!found){
        if (b) { bbflag[l] = 1; bb[l][c] = 1; return 0; }
        v[v.size()-1]+=l+2-cl;
        bb[l][c]=4;
      }
    }
  }
  return true;
}


bool CrossSolver::TestLine(int l) {
  SetFirst(l);
  do {
    if (TestPosition(l)) {
      if (l==L[0]-1) return true;
      else if (TestLine(l+1)) return true;
    }
  } while (GetNext(l));
  return 0;
}

void CrossSolver::FixLastColumnGroup() {
  for (int i=0;i<L[1];i++) {
    int l=0;
    bool b=0;
    for (int k=0;k<a[1][i].size();k++,b=!b) 
      for (int j=0;j<a[1][i][k];j++,l++) {
        if (l>=L[0]) { 
          if (k<a[1][i].size()-1) noSolution();
          a[1][i][k]=j;
        }
      }
  }
}

void CrossSolver::SolveCrosspoint() { 
  if (TestLine(0)) {
    FixLastColumnGroup();
    Solved = true;
  }
  else noSolution();
}

void WriteCrosspoint(int w) {
  if (!ways[w].Solution) {
    NoSolution();    
    return;
  }
  if (w==1 || w==3) {
    reverse(ways[w].a[0].begin(),ways[w].a[0].end());
  }
  if (w==5 || w==7) {
    for (int i=0;i<ways[w].L[1];i++) 
      reverse(ways[w].a[1][i].begin(),ways[w].a[1][i].end());
  }
  if (w==6 || w==7) {
    reverse(ways[w].a[1].begin(),ways[w].a[1].end());
    reverse(ways[w].s[1].begin(),ways[w].s[1].end());
  }
  if (w==2 || w==3) {
    for (int i=0;i<ways[w].L[0];i++) 
      reverse(ways[w].a[0][i].begin(),ways[w].a[0][i].end());
  }
  if (asciiart) {
    string buf;
    for (int i=0; i<ways[w].L[w<4?0:1];i++) {
      if (!(i%2)) buf = "";
      bool empty  = true;
      int p = 0;
      for (int k=0;k<ways[w].a[w<4?0:1][i].size();k++,empty=!empty)
        for (int j=0;j<ways[w].a[w<4?0:1][i][k];j++) {
          if (!empty) {
            if (i%2) {
              if (buf[p] == '‹') buf[p] = '';
              else buf[p] = 'Œ';
            }
            else buf.push_back('‹');
          }
          else if (!(i%2)) buf.push_back(' ');
          ++p;
        }
      if (i%2 ||i == ways[w].L[w<4?0:1]-1) cout << buf << endl;
    }
  }
  else for (int i=0;i<ways[w].L[w<4?0:1];i++) {
    char z='.';
    for (int k=0;k<ways[w].a[w<4?0:1][i].size();k++,z=(z=='.'?'*':'.'))
      for (int j=0;j<ways[w].a[w<4?0:1][i][k];j++) cout << z;
    cout << endl;
  }
}

bool ReadCrosspoint() {
  for (int w=0;w<8;w++) {
    for (int k=0;k<2;k++) {
      ways[w].a[k].clear();
      ways[w].s[k].clear();
      ways[w].L[k]=0;
    }
  }
  string sbuf;
  int i=0;
  while (getline(cin,sbuf)) {
    if (sbuf=="*") if (i) break; else { i++; continue; }
    
    for (int w=0;w<8;w++) {
      ways[w].a[w<4?i:(i?0:1)].push_back(vector<int>());
      ways[w].a[w<4?i:(i?0:1)][ways[w].L[w<4?i:(i?0:1)]].push_back(0);
    }    
    int sum = 0;
    int buf;
    for (istringstream isbuf(sbuf);isbuf >> buf;sum+=buf) {
      for (int w=0;w<8;w++) {
        ways[w].a[w<4?i:(i?0:1)][ways[w].L[w<4?i:(i?0:1)]].push_back(buf);
        ways[w].a[w<4?i:(i?0:1)][ways[w].L[w<4?i:(i?0:1)]].push_back(1);
      }       
    }
    for (int w=0;w<8;w++) {
      ways[w].s[w<4?i:(i?0:1)].push_back(sum);
    }
    for (int w=0;w<8;w++) {
      ways[w].L[w<4?i:(i?0:1)]++;
    }
  }
  if (ways[0].L[0]<1||ways[0].L[1]<1) {
    NoSolution();
    return 0;
  }
  
  for (int w=1;w<8;w++) {
    if (w==1 || w==3 || w==5 || w==7 ) {
      reverse(ways[w].a[0].begin(),ways[w].a[0].end());
      reverse(ways[w].s[0].begin(),ways[w].s[0].end());
      for (int i=0;i<ways[w].L[1];i++) 
        reverse(ways[w].a[1][i].begin(),ways[w].a[1][i].end());
    }
    if (w==2 || w==3 || w==6 || w==7) {
      reverse(ways[w].a[1].begin(),ways[w].a[1].end());
      reverse(ways[w].s[1].begin(),ways[w].s[1].end());
      for (int i=0;i<ways[w].L[0];i++) 
        reverse(ways[w].a[0][i].begin(),ways[w].a[0][i].end());
    }
  }
    
  for (int w=0;w<8;w++) {
    ways[w].bb.clear();
    ways[w].bb.resize(ways[w].L[0]);
    for (int i=0;i<ways[w].L[0];i++) ways[w].bb[i].resize(ways[w].L[1],0);
    ways[w].bbflag.clear();
    ways[w].bbflag.resize(ways[w].L[0],0);
    ways[w].Solved = 0;
    ways[w].Solution = true;
  }
  return true;
}

void* ThreadProc(void* way) {
  static_cast<CrossSolver*>(way)->SolveCrosspoint();
}

int main(int argc, char ** argv) {
  if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'a' && argv[1][2] == '\0')
    asciiart = true;
  bool firstCrossPoint = true;
  while (!cin.eof()) {
    if (!firstCrossPoint) cout << "\n";
    else firstCrossPoint = false;
    if (!ReadCrosspoint()) continue; 
    pthread_t t[8];
    for (int i=0;i<8;i++) pthread_create(&t[i],0,ThreadProc,&ways[i]);
    int found=-1; 
    do {
      sleep(1);
      for (int i=0;i<8;i++) if (ways[i].Solved) {
        found=i;
        break;
      }
    } while (found==-1);
    for (int i=0;i<8;i++) ways[i].Solved=true;
    WriteCrosspoint(found);
    for (int i=0;i<8;i++) pthread_join(t[i],0);
  }
}
