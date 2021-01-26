#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#include <eigen3/Eigen/QR>
#include <stdio.h>
#include <vector>

using namespace std;
vector<float>polyfiteigen(const vector<float> &xv, const vector<float> &yv, int order)
{
	Eigen::initParallel();
	Eigen::MatrixXf A = Eigen::MatrixXf::Ones(xv.size(), order + 1);
	Eigen::VectorXf yv_mapped = Eigen::VectorXf::Map(&yv.front(), yv.size());
	Eigen::VectorXf xv_mapped = Eigen::VectorXf::Map(&xv.front(), xv.size());
	Eigen::VectorXf result;

	assert(xv.size() == yv.size());
	assert(xv.size() >= order + 1);

	for (int j = 1; j < order + 1; j++)
	{
		A.col(j) = A.col(j-1).cwiseProduct(xv_mapped);
	}
	
	result = A.householderQr().solve(yv_mapped);
	vector<float> coeff;
	coeff.resize(order + 1);
	for (size_t i = 0; i < order + 1; i++)
		coeff[i] = result[i];

	return coeff;
}
//                       # Copyright For polyfiteigen: END                          //


vector<float>polyvaleigen(const vector<float>& oCoeff,
	const vector<float>& oX)
{
	int nCount = int(oX.size());
	int nDegree = int(oCoeff.size());
	vector<float>oY(nCount);
	
	for (int i = 0; i < nCount; i++)
	{
		float nY = 0;
		float nXT = 1;
		float nX = oX[i];
		for (int j = 0; j < nDegree; j++)
		{
			nY += oCoeff[j] * nXT;
			nXT *= nX;
		}
		oY[i] = nY;
		
	}

	return oY;
}


int print_eigen(Eigen::MatrixX3d m)
{
    // Eigen Matrices do have rule to print them with std::cout
    std::cout << m << std::endl;
    return 0;
}

int main()
{
    Eigen::Matrix3d test; //3 by 3 double precision matrix initialization

    // Let's make it a symmetric matrix
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
            test(i,j) = (i+1)*(1+j);
    }

    // Print
    print_eigen(test);

    return 0;
}