int main()
{
	printf("Enter images directory path:\n");
	char* dir;
	scanf("%s", &dir);

	printf("Enter images prefix:\n");
	char* prefix;
	scanf("%s", &prefix);

	printf("Enter number of images:\n");
	int n = 0;
	scanf("%d", &n);
	if (n < 1)
	{
		printf("An error occurred - invalid number of images\n");
	}

	printf("Enter images suffix:\n");
	char* suffix;
	scanf("%s", &suffix);

	printf("Enter number of bins:\n");
	int nBins = 0;
	scanf("%d", &nBins);
	if (n < 1)
	{
		printf("An error occurred - invalid number of bins\n");
	}

	printf("Enter number of features:\n");
	int maxNFeatures = 0;
	scanf("%d", &maxNFeatures);
	if (n < 1)
	{
		printf("An error occurred - invalid number of features\n");
	}

	//7
	int* nFeaturesPerImage;
	nFeaturesPerImage = malloc(n*sizeof(int));
	int*** rgb;
	rgb = malloc(n*sizeof(int));
	double*** sift;
	sift = malloc(n*sizeof(int));


	for (int i = 0 ; i < n ; i++)
	{
		char* currentDir = dir+prefix+i+suffix;
		rgb[i] = spGetRGBHist(currentDir, nBins);
		sift[i] = spGetSiftDescriptors(currentDir,  maxNFeatures, &(nFeaturesPerImage[i]));
	}

	//8
	printf("Enter a query image or # to terminate:\n");
	char* query;
	scanf("%s", &query);

	if(strcmp("#",query) == 0)
	{
		printf("Exiting...\n");
	}

	free_data(***rgb,n,3);
	free_data(***sift,n, maxNFeatures);

}


int ***alloc_data(size_t xlen, size_t ylen, size_t zlen)
{
    int ***p;
    size_t i, j;

    if ((p = malloc(xlen * sizeof *p)) == NULL) {
        perror("malloc 1");
        return NULL;
    }

    for (i=0; i < xlen; ++i)
        p[i] = NULL;

    for (i=0; i < xlen; ++i)
        if ((p[i] = malloc(ylen * sizeof *p[i])) == NULL) {
            perror("malloc 2");
            free_data(p, xlen, ylen);
            return NULL;
        }

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            if ((p[i][j] = malloc(zlen * sizeof *p[i][j])) == NULL) {
                perror("malloc 3");
                free_data(p, xlen, ylen);
                return NULL;
            }

    return p;
}

void free_data(int ***data, size_t xlen, size_t ylen)
{
    size_t i, j;

    for (i=0; i < xlen; ++i) {
        if (data[i] != NULL) {
            for (j=0; j < ylen; ++j)
                free(data[i][j]);
            free(data[i]);
        }
    }
    free(data);
}

