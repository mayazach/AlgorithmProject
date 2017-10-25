#ifndef CURVE
#define CURVE

struct Curve{
	int m;
	std::string id;
	int dimension;
	double** points;
};

struct Neighbor{
	double dist;
	std::string id;
};

#endif