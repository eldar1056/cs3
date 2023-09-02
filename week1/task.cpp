#include <cmath>
#include <set>

using namespace std;

struct Point_3D
{
	double x, y, z;
};

Point_3D middle(Point_3D const& A, Point_3D const& B)
{
	return { 0.5 * (A.x + B.x), 0.5 * (A.y + B.y), 0.5 * (A.z + B.z) };
}

double distance(Point_3D const& A, Point_3D const& B)
{
	return sqrt(pow(A.x-B.x, 2) + pow(A.y - B.y, 2) + pow(A.z - B.z, 2));
}

Point_3D center_of_mass(Point_3D const* const begin, Point_3D const* const end)
{
	Point_3D sum = *begin;
	if (begin == end)
		return sum;

	int counter = 1;
	for (auto* it = begin + 1; it != end; it++)
	{
		sum.x += it->x;
		sum.y += it->y;
		sum.z += it->z;
		counter++;
	}

	return {sum.x/counter, sum.y/counter, sum.z/counter};
}

struct PointCloud
{
	Point_3D* begin = nullptr;
	unsigned size = 0u;

};

PointCloud create_cloud(size_t n)
{
	PointCloud pc;

	pc.begin = new Point_3D[n];
	pc.size = n;

	return pc;
}

void delete_cloud(const PointCloud& pc)
{
	for (int i = 0; i < pc.size; i++)
		delete (pc.begin + i);
	delete[] pc.begin;
}

bool operator<(const Point_3D& left, const Point_3D& right)
{
	if (left.x < right.x or (left.x == right.x and (left.y < right.y or (left.y == right.y and left.z < right.z))))
		return true;
	return false;
}

PointCloud merge_clouds(const PointCloud& A, const PointCloud& B)
{
	set<Point_3D> points;
	for (int i = 0; i < A.size; i++)
		points.insert(*(A.begin + i));
	for (int i = 0; i < B.size; i++)
			points.insert(*(B.begin + i));

	auto C = create_cloud(points.size());

	int counter = 0;
	for (const auto point : points)
		*(C.begin + counter++) = point;

	return C;
}
