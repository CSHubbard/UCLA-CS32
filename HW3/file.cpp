//
//#include <iostream>
//#include <string>
//using namespace std;

//your declarations and implementations would go here

//file class
class file
{
public:
	file(string name)
		:f_name(name) {}
	
	virtual ~file() {}

	string name() const
	{
		return f_name;
	}

	virtual void open() const = 0;

	virtual void redisplay() const
	{
		cout << "refresh the screen";
	}

private:
	string f_name;
};
////////
class textmsg : public file
{
public:
	textmsg(string name)
		:file(name) { }

	virtual ~textmsg() 
	{
		cout << "destroying " << name() << ", a text message" <<endl;
	}

	virtual void open() const
	{
		cout << "open text message";
	}

private:
};
/////////
class video : public file
{
public:
	video(string name, int length)
		:file(name) 
	{
		v_leng = length;
	}

	virtual ~video() 
	{
		cout << "destroying " << name() << ", a video" <<endl;
	}

	virtual void open() const
	{
		cout << "play " << v_leng << " second video";
	}

	virtual void redisplay() const
	{
		cout << "replay video";
	}

private:
	int v_leng;
};
////////////////
class picture : public file
{
public:
	picture(string name)
		:file(name) {}
	~picture()
	{
		cout << "destroying the picture " << name() << endl;
	}

	virtual void open() const
	{
		cout << "show picture";
	}

private:
};

//void openandredisplay(const file* f)
//{
//	cout << f->name() << ": ";
//	f->open();
//	cout << endl << "redisplay: ";
//	f->redisplay();
//	cout << endl;
//}

//int main()
//{
//	file* files[4];
//	files[0] = new textmsg("fromfred.txt");
//	// videos have a name and running time
//	files[1] = new video("goblin.mpg", 3780);
//	files[2] = new picture("kitten.jpg");
//	files[3] = new picture("baby.jpg");
//
//	for (int k = 0; k < 4; k++)
//		openandredisplay(files[k]);
//
//	// clean up the files before exiting
//	cout << "cleaning up." << endl;
//	for (int k = 0; k < 4; k++)
//		delete files[k];
//}
//
//  fromfred.txt: open text message
//  redisplay: refresh the screen
//	goblin.mpg: play 3780 second video
//	redisplay: replay video
//	kitten.jpg: show picture
//	redisplay: refresh the screen
//	baby.jpg: show picture
//	redisplay: refresh the screen
//	cleaning up.
//	destroying fromfred.txt, a text message
//	destroying goblin.mpg, a video
//	destroying the picture kitten.jpg
//	destroying the picture baby.jpg