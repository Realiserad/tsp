#ifndef __TOUR
#define __TOUR

class Tour {
	int *_tour;
	int *_index;
	int _size;
	
	void copy_construct(const Tour&);
	void move_construct(Tour&);
	void create_index(int);

	public:
	~Tour();
	Tour(int*, int);
	Tour(const Tour&);
	Tour& operator=(Tour&);
	Tour(Tour&&);
	Tour& operator=(Tour&&);
	void swap(int, int);
	double length(double**) const;
	int size() const;
	void transform();
	int index_of(int) const;
	int operator[](int) const;
	void set(int, int);
	void print() const;
};

#endif
