#include<cstdio>
#include<cmath>
#include<algorithm>
#include<cstring>
#include<queue>
#include<iostream>
#include<Windows.h>
#include<fstream>
#include<io.h>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
double v[2][5][100][100];
double pic[2][100][100];
struct node
{
  double w;
  int n;
};
node dis[20];

void GetAllFiles(string path, vector<string>& files)
{

	long   hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

}

void readpic(int num1,int mark,FILE *stream)
{
	if(stream==NULL)
	{
	    printf("open file failure\n");
		return ;
	}
	//else printf("success\n");
	int sizeFileHeader=sizeof(BITMAPFILEHEADER);
	int sizeInfoHeader=sizeof(BITMAPINFOHEADER);
	BITMAPFILEHEADER* bitmapFileHeader=new BITMAPFILEHEADER[sizeFileHeader+1];
	BITMAPINFOHEADER* bitmapInfoHeader=new BITMAPINFOHEADER[sizeInfoHeader+1];
	memset(bitmapFileHeader,0,sizeFileHeader+1);
	memset(bitmapInfoHeader,0,sizeInfoHeader+1);
	fread(bitmapFileHeader,sizeof(char),sizeFileHeader,stream);
	fseek(stream,sizeFileHeader,0);
	fread(bitmapInfoHeader,sizeof(char),sizeInfoHeader,stream);
	fseek(stream,sizeInfoHeader+sizeFileHeader,0);
	RGBQUAD* pRgbQuards=new RGBQUAD[256];
	for (int k=0;k<256;k++)
	{
		fread(&pRgbQuards[k],sizeof(RGBQUAD),1,stream);
	}
	int count1=(((bitmapInfoHeader->biWidth)*8+31)/32)*4-bitmapInfoHeader->biWidth*(bitmapInfoHeader->biBitCount/8);
	BYTE* tempData=new BYTE[count1+1];
	memset(tempData,0,count1+1);
	fseek(stream,sizeFileHeader+sizeInfoHeader+256*sizeof(RGBQUAD),0);
	BYTE** data=new BYTE*[bitmapInfoHeader->biHeight];
	//printf("%d %d\n",bitmapInfoHeader->biHeight,bitmapInfoHeader->biWidth);
	//int lx,ly;
	//lx=bitmapInfoHeader->biHeight;
	//ly=bitmapInfoHeader->biWidth;
	double Max=1.0;
	for(int i=0;i<bitmapInfoHeader->biHeight;i++)
	{
		data[i]=new BYTE[bitmapInfoHeader->biWidth];
		for (int j=0;j<bitmapInfoHeader->biWidth;j++)
		{
			fread(&data[i][j],sizeof(char),1,stream);
           if(mark==0)
           {
               v[num1/5][num1%5][i][j]=data[i][j]*1.0;
               Max=max(v[num1/5][num1%5][i][j],Max);
           }
           else
           {
               pic[num1][i][j]=data[i][j]*1.0;
               Max=max(pic[num1][i][j],Max);
           }
		}
		for (int n=0;n<count1;n++)
		{
			fread(&tempData[n],sizeof(char),1,stream);
		}
		//printf("\n");
	}
	//printf("num1:%d mark:%d Max:%llf\n",num1,mark,Max);
	int i,j;
    for(i=0;i<100;i++)
        for(j=0;j<100;j++)
            {
                if(mark==0)
                v[num1/5][num1%5][i][j]=v[num1/5][num1%5][i][j]/Max;
                else pic[num1][i][j]=pic[num1][i][j]/Max;
            }
	return;
}

double dist(double x,double y)
{
    return abs(x-y)*abs(x-y);
}

bool cmp(node x,node y)
{
    return x.w<y.w;
}

int judge(int num)
{
    int i,j,k,h;
    double l;
    int c=0;
    for(k=0;k<2;k++)
       {
        for(h=0;h<5;h++)
         {
            l=0.0;
            for(i=0;i<100;i++)
               for(j=0;j<100;j++)
                l+=dist(pic[num][i][j],v[k][h][i][j]);
            l=sqrt(l);
            dis[c].w=l;
            dis[c++].n=k;
         }
      }
    sort(dis,dis+c,cmp);//?
    int p;//generally p<sqrt(n) n is the num of examples
    /*printf("num:%d\n",num);
    for(p=0;p<10;p++)
        printf("%d   %llf\n",dis[p].n,dis[p].w);*/
    //printf("num:%d\n",num);
    /*for(i=0;i<10;i++)
        printf("i:%d n:%d w:%llf\n",i,dis[i].n,dis[i].w);*/
    int mark[5];
    //printf("num:%d\n",num);
    for(p=0;p<3;p++)
    {
       // printf("p:%d %d %llf\n",p,dis[p].n,dis[p].w);
        mark[p]=dis[p].n;
    }
    if(mark[1]==mark[2]&&mark[1]!=mark[0]) return mark[1];
    else return mark[0];
}

int main()
{
    int i,j;
  //int num[10][10];
  string filePath ="C:\\Users\\Administrator\\Desktop\\face\\training";
  vector<string> files;
  GetAllFiles(filePath, files);
  int size = files.size();
  for (i = 1; i<size; i++)
  {
	    FILE *stream;
        string s1="C:/Users/Administrator/Desktop/face/training/";
        string s2=files[i].data();
        string s=s1+s2;
        const char * filename=s.c_str();
        cout<<filename<<endl;
        stream=fopen(filename,"rb");
        readpic(i,0,stream);
       // fclose(stream);
  }
  string filePath1="C:\\Users\\Administrator\\Desktop\\face\\test";
  vector<string> files1;
  GetAllFiles(filePath1, files1);
  int size1=files1.size();
  for(i=0;i<size1;i++)
  {

      FILE *fp;
      string s1="C:/Users/Administrator/Desktop/face/test/";
      string s2=files1[i].data();
      const char * filename1=(s1+s2).data();
      fp=fopen(filename1,"rb");
      cout<<filename1<<endl;
      readpic(i,1,fp);
      fclose(fp);
      int l=judge(i);
      cout<<files1[i]<<" is ";
      if(l==0) printf("my sister\n");
      else printf("my idol\n");
      //<<l<<endl;
  }
  return 0;
}
