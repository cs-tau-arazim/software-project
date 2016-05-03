int main()
{
	printf("Enter images directory path:\n");
	char* dir = 0;
	scanf("%s", &dir);

	printf("Enter images prefix:\n");
	char* prefix = 0;
	scanf("%s", &prefix);

	printf("Enter number of images:\n");
	int n = 0;
	scanf("%d", &n);
	if (n < 1)
	{
		printf("An error occurred - invalid number of images\n");
	}

	printf("Enter images suffix:\n");
	char* suffix = 0;
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

	for (int i = 0 ; i < n ; i++)
	{
		char* currentDir = dir+prefix+i+suffix; // TODO
		// TODO save rgb hist and features for each image
	}
}

