/*// CPP program to illustrate
// Scanline Polygon fill Algorithm

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define maxHt 800
#define maxWd 600
#define maxVer 10000

FILE *fp;

// Start from lower left corner
typedef struct edgebucket
{
	int ymax; //max y-coordinate of edge
	float xofymin; //x-coordinate of lowest edge point updated only in aet
	float slopeinverse;
}EdgeBucket;

typedef struct edgetabletup
{
	// the array will give the scanline number
	// The edge table (ET) with edges entries sorted
	// in increasing y and x of the lower end

	int countEdgeBucket; //no. of edgebuckets
	EdgeBucket buckets[maxVer];
}EdgeTableTuple;

EdgeTableTuple EdgeTable[maxHt], ActiveEdgeTuple;


// Scanline Function
void initEdgeTable()
{
	int i;
	for (i=0; i<maxHt; i++)
	{
		EdgeTable[i].countEdgeBucket = 0;
	}

	ActiveEdgeTuple.countEdgeBucket = 0;
}


void printTuple(EdgeTableTuple *tup)
{
	int j;

	if (tup->countEdgeBucket)
		printf("\nCount %d-----\n",tup->countEdgeBucket);

		for (j=0; j<tup->countEdgeBucket; j++)
		{
			printf(" %d+%.2f+%.2f",
			tup->buckets[j].ymax, tup->buckets[j].xofymin,tup->buckets[j].slopeinverse);
		}
}

void printTable()
{
	int i,j;

	for (i=0; i<maxHt; i++)
	{
		if (EdgeTable[i].countEdgeBucket)
			printf("\nScanline %d", i);

		printTuple(&EdgeTable[i]);
	}
}


/* Function to sort an array using insertion sort*//*
void insertionSort(EdgeTableTuple *ett)
{
	int i,j;
	EdgeBucket temp;

	for (i = 1; i < ett->countEdgeBucket; i++)
	{
		temp.ymax = ett->buckets[i].ymax;
		temp.xofymin = ett->buckets[i].xofymin;
		temp.slopeinverse = ett->buckets[i].slopeinverse;
		j = i - 1;

	while ((temp.xofymin < ett->buckets[j].xofymin) && (j >= 0))
	{
		ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
		ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
		ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
		j = j - 1;
	}
	ett->buckets[j + 1].ymax = temp.ymax;
	ett->buckets[j + 1].xofymin = temp.xofymin;
	ett->buckets[j + 1].slopeinverse = temp.slopeinverse;
	}
}


void storeEdgeInTuple (EdgeTableTuple *receiver,int ym,int xm,float slopInv)
{
	// both used for edgetable and active edge table..
	// The edge tuple sorted in increasing ymax and x of the lower end.
	(receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
	(receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
	(receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

	// sort the buckets
	insertionSort(receiver);

	(receiver->countEdgeBucket)++;


}

void storeEdgeInTable (int x1,int y1, int x2, int y2)
{
	float m,minv;
	int ymaxTS,xwithyminTS, scanline; //ts stands for to store

	if (x2==x1)
	{
		minv=0.000000;
	}
	else
	{
	m = ((float)(y2-y1))/((float)(x2-x1));

	// horizontal lines are not stored in edge table
	if (y2==y1)
		return;

	minv = (float)1.0/m;
	printf("\nSlope string for %d %d & %d %d: %f",x1,y1,x2,y2,minv);
	}

	if (y1>y2)
	{
		scanline=y2;
		ymaxTS=y1;
		xwithyminTS=x2;
	}
	else
	{
		scanline=y1;
		ymaxTS=y2;
		xwithyminTS=x1;
	}
	// the assignment part is done..now storage..
	storeEdgeInTuple(&EdgeTable[scanline],ymaxTS,xwithyminTS,minv);


}

void removeEdgeByYmax(EdgeTableTuple *Tup,int yy)
{
	int i,j;
	for (i=0; i< Tup->countEdgeBucket; i++)
	{
		if (Tup->buckets[i].ymax == yy)
		{
			printf("\nRemoved at %d",yy);

			for ( j = i ; j < Tup->countEdgeBucket -1 ; j++ )
				{
				Tup->buckets[j].ymax =Tup->buckets[j+1].ymax;
				Tup->buckets[j].xofymin =Tup->buckets[j+1].xofymin;
				Tup->buckets[j].slopeinverse = Tup->buckets[j+1].slopeinverse;
				}
				Tup->countEdgeBucket--;
			i--;
		}
	}
}


void updatexbyslopeinv(EdgeTableTuple *Tup)
{
	int i;

	for (i=0; i<Tup->countEdgeBucket; i++)
	{
		(Tup->buckets[i]).xofymin =(Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
	}
}


void ScanlineFill()
{
	/* Follow the following rules:
	1. Horizontal edges: Do not include in edge table
	2. Horizontal edges: Drawn either on the bottom or on the top.
	3. Vertices: If local max or min, then count twice, else count
		once.
	4. Either vertices at local minima or at local maxima are drawn.*/
/*

	int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

	// we will start from scanline 0;
	// Repeat until last scanline:
	for (i=0; i<maxHt; i++)//4. Increment y by 1 (next scan line)
	{

		// 1. Move from ET bucket y to the
		// AET those edges whose ymin = y (entering edges)
		for (j=0; j<EdgeTable[i].countEdgeBucket; j++)
		{
			storeEdgeInTuple(&ActiveEdgeTuple,EdgeTable[i].buckets[j].
					ymax,EdgeTable[i].buckets[j].xofymin,
					EdgeTable[i].buckets[j].slopeinverse);
		}
		printTuple(&ActiveEdgeTuple);

		// 2. Remove from AET those edges for
		// which y=ymax (not involved in next scan line)
		removeEdgeByYmax(&ActiveEdgeTuple, i);

		//sort AET (remember: ET is presorted)
		insertionSort(&ActiveEdgeTuple);

		printTuple(&ActiveEdgeTuple);

		//3. Fill lines on scan line y by using pairs of x-coords from AET
		j = 0;
		FillFlag = 0;
		coordCount = 0;
		x1 = 0;
		x2 = 0;
		ymax1 = 0;
		ymax2 = 0;
		while (j<ActiveEdgeTuple.countEdgeBucket)
		{
			if (coordCount%2==0)
			{
				x1 = (int)(ActiveEdgeTuple.buckets[j].xofymin);
				ymax1 = ActiveEdgeTuple.buckets[j].ymax;
				if (x1==x2)
				{
				/* three cases can arrive-
					1. lines are towards top of the intersection
					2. lines are towards bottom
					3. one line is towards top and other is towards bottom

					if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
					{
						x2 = x1;
						ymax2 = ymax1;
					}

					else
					{
						coordCount++;
					}
				}

				else
				{
						coordCount++;
				}
			}
			else
			{
				x2 = (int)ActiveEdgeTuple.buckets[j].xofymin;
				ymax2 = ActiveEdgeTuple.buckets[j].ymax;

				FillFlag = 0;

				// checking for intersection...
				if (x1==x2)
				{
				/*three cases can arrive-
					1. lines are towards top of the intersection
					2. lines are towards bottom
					3. one line is towards top and other is towards bottom

					if (((x1==ymax1)&&(x2!=ymax2))||((x1!=ymax1)&&(x2==ymax2)))
					{
						x1 = x2;
						ymax1 = ymax2;
					}
					else
					{
						coordCount++;
						FillFlag = 1;
					}
				}
				else
				{
						coordCount++;
						FillFlag = 1;
				}


			if(FillFlag)
			{
				//drawing actual lines...
				glColor3f(0.0f,0.7f,0.0f);

				glBegin(GL_LINES);
				glVertex2i(x1,i);
				glVertex2i(x2,i);
				glEnd();
				glFlush();

				// printf("\nLine drawn from %d,%d to %d,%d",x1,i,x2,i);
			}

		}

		j++;
	}


	// 5. For each nonvertical edge remaining in AET, update x for new y
	updatexbyslopeinv(&ActiveEdgeTuple);
}


printf("\nScanline filling complete");

}


void myInit(void)
{

	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluOrtho2D(0,maxHt,0,maxWd);
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawPolyDino()
{

	glColor3f(1.0f,0.0f,0.0f);
	int count = 0,x1,y1,x2,y2;
	rewind(fp);
	while(!feof(fp) )
	{
		count++;
		if (count>2)
		{
			x1 = x2;
			y1 = y2;
			count=2;
		}
		if (count==1)
		{
			fscanf(fp, "%d,%d", &x1, &y1);
		}
		else
		{
			fscanf(fp, "%d,%d", &x2, &y2);
			printf("\n%d,%d", x2, y2);
			glBegin(GL_LINES);
				glVertex2i( x1, y1);
				glVertex2i( x2, y2);
			glEnd();
			storeEdgeInTable(x1, y1, x2, y2);//storage of edges in edge table.


			glFlush();
		}
	}


}

void drawDino(void)
{
	initEdgeTable();
	drawPolyDino();
	printf("\nTable");
	printTable();

	ScanlineFill();//actual calling of scanline filling..
}

void main(int argc, char** argv)
{
	fp=fopen ("PolyDino.txt","r");
	if ( fp == NULL )
	{
		printf( "Could not open file" ) ;
		return;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(maxHt,maxWd);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Scanline filled dinosaur");
	myInit();
	glutDisplayFunc(drawDino);

	glutMainLoop();
	fclose(fp);
}
*/
// main.c — Scan‑Line‑Filled Cube (front, top & side faces)

#include <GL/glut.h>    // GLUT and OpenGL headers :contentReference[oaicite:0]{index=0}
#include <math.h>       // ceilf(), floorf() prototypes :contentReference[oaicite:1]{index=1}
#include <stdlib.h>

#define MAX_HT    500    // Maximum scan‐line height
#define MAX_EDGES 100    // Max edges per polygon

//------------------------------------------------------------------------------
// Data structures
//------------------------------------------------------------------------------
typedef struct {
    int   ymax;           // upper y of edge
    float xofymin;        // current x intersect (starts at x at ymin)
    float slopeInv;       // Δx/Δy
} EdgeBucket;

typedef struct {
    int          count;
    EdgeBucket   buckets[MAX_EDGES];
} EdgeTableTuple;

static EdgeTableTuple ET[MAX_HT];  // global Edge Table :contentReference[oaicite:2]{index=2}
static EdgeTableTuple AET;         // Active Edge Table

//------------------------------------------------------------------------------
// Initialize edge table and AET
//------------------------------------------------------------------------------
void initTables(void) {
    for (int i = 0; i < MAX_HT; i++)
        ET[i].count = 0;
    AET.count = 0;
}

//------------------------------------------------------------------------------
// Insert one edge into an edge‐table tuple (sorted by xofymin)
//------------------------------------------------------------------------------
void insertEdge(EdgeTableTuple *t, int ymax, float xofymin, float slopeInv) {
    int i = t->count++;
    t->buckets[i].ymax     = ymax;
    t->buckets[i].xofymin  = xofymin;
    t->buckets[i].slopeInv = slopeInv;
    // insertion sort backward
    for (; i > 0 && t->buckets[i].xofymin < t->buckets[i-1].xofymin; i--) {
        EdgeBucket tmp      = t->buckets[i];
        t->buckets[i]       = t->buckets[i-1];
        t->buckets[i-1]     = tmp;
    }
}

//------------------------------------------------------------------------------
// Build ET for a single face (n vertices in vertices[][2])
//------------------------------------------------------------------------------
void buildET(int vertices[][2], int n) {
    for (int i = 0; i < n; i++) {
        int x1 = vertices[i][0],   y1 = vertices[i][1];
        int x2 = vertices[(i+1)%n][0], y2 = vertices[(i+1)%n][1];
        if (y1 == y2) continue;          // skip horizontal edges :contentReference[oaicite:3]{index=3}
        int ymin = y1 < y2 ? y1 : y2;
        int ymax = y1 < y2 ? y2 : y1;
        float x_at_ymin = (y1 < y2 ? x1 : x2);
        float slopeInv  = (float)(x2 - x1) / (float)(y2 - y1);
        insertEdge(&ET[ymin], ymax, x_at_ymin, slopeInv);
    }
}

//------------------------------------------------------------------------------
// Remove edges from AET whose ymax == scanY
//------------------------------------------------------------------------------
void removeExpired(int scanY) {
    int dst = 0;
    for (int i = 0; i < AET.count; i++) {
        if (AET.buckets[i].ymax != scanY)
            AET.buckets[dst++] = AET.buckets[i];
    }
    AET.count = dst;
}

//------------------------------------------------------------------------------
// Advance each AET edge’s x by slopeInv
//------------------------------------------------------------------------------
void updateAET(void) {
    for (int i = 0; i < AET.count; i++)
        AET.buckets[i].xofymin += AET.buckets[i].slopeInv;
}

//------------------------------------------------------------------------------
// Scan‑line fill one polygon face
//------------------------------------------------------------------------------
void scanlineFill(int verts[][2], int n, float r, float g, float b) {
    buildET(verts, n);    // populate ET :contentReference[oaicite:4]{index=4}
    AET.count = 0;
    glColor3f(r, g, b);
    for (int y = 0; y < MAX_HT; y++) {
        // 1) move ET[y] → AET
        for (int i = 0; i < ET[y].count; i++) {
            EdgeBucket *e = &ET[y].buckets[i];
            insertEdge(&AET, e->ymax, e->xofymin, e->slopeInv);
        }
        // 2) remove expired edges
        removeExpired(y);   // :contentReference[oaicite:5]{index=5}
        // 3) fill spans
        glBegin(GL_POINTS);
        for (int i = 0; i+1 < AET.count; i += 2) {
            int x1 = (int)ceilf (AET.buckets[i].xofymin);
            int x2 = (int)floorf(AET.buckets[i+1].xofymin);
            for (int x = x1; x <= x2; x++)
                glVertex2i(x, y);
        }
        glEnd();
        // 4) update x for next y
        updateAET();         // :contentReference[oaicite:6]{index=6}
    }
}

//------------------------------------------------------------------------------
// GLUT display callback: draw cube faces
//------------------------------------------------------------------------------
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Define 2D “cube” faces
    int front[4][2] = {{150,150},{350,150},{350,350},{150,350}};
    int top  [4][2] = {{150,350},{350,350},{300,400},{100,400}};
    int side [4][2] = {{350,150},{350,350},{300,400},{300,200}};

    initTables();
    scanlineFill(front, 4, 0.8f, 0.2f, 0.2f); // dark red
    scanlineFill(top,   4, 0.9f, 0.9f, 0.2f); // yellow
    scanlineFill(side,  4, 0.2f, 0.2f, 0.8f); // blue

    // Outline edges in black
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
      // front
      for (int i=0;i<4;i++) glVertex2iv(front[i]);
    glEnd();
    glBegin(GL_LINE_LOOP);
      // top
      for (int i=0;i<4;i++) glVertex2iv(top[i]);
    glEnd();
    glBegin(GL_LINE_LOOP);
      // side
      for (int i=0;i<4;i++) glVertex2iv(side[i]);
    glEnd();

    glutSwapBuffers();
}

//------------------------------------------------------------------------------
// GLUT reshape: setup orthographic projection
//------------------------------------------------------------------------------
void reshape(int w,int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0, MAX_HT, 0, MAX_HT);  // match scan‐line range :contentReference[oaicite:7]{index=7}
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(MAX_HT, MAX_HT);
    glutCreateWindow("Scan‑Line Filled Cube");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
