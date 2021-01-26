#include<iostream>
#include<string>
#include<map>

typedef unsigned char uchar;
class pixel
{
    private:
        uchar r;
        uchar g;
        uchar b;
    public:
        pixel();
        pixel(uchar* rgb);
        pixel(uchar r,uchar g, uchar b);
        void set_channels(uchar* rgb);
        void set_red(uchar r);
        void set_green(uchar g);
        void set_blue(uchar b);
        uchar red()const{return r;};
        uchar green()const{return g;};
        uchar blue()const{return b;};
        uchar* channels()const;
};

pixel::pixel(uchar* rgb)
{
    r = rgb[0];
    g = rgb[1];
    b = rgb[2];
}

pixel::pixel(uchar red,uchar green,uchar blue)
{
    this->r = red;
    this->g = green;
    this->b = blue;
}

pixel::pixel()
{
    r = 255;
    g = 255;
    b = 255;
}

void pixel::set_channels(uchar* rgb)
{
    r = rgb[0];
    g = rgb[1];
    b = rgb[2];
}

void pixel::set_red(uchar r)
{
    this->r = r;
}
void pixel::set_green(uchar g)
{
    this->g= g;
}
void pixel::set_blue(uchar b)
{
    this->b= b;
}

uchar* pixel::channels()const
{
    uchar *RGB = new uchar[3];
    RGB[0] = r; 
    RGB[1] = g; 
    RGB[2] = b; 
    return RGB;
}
/*
class colors
{
    private:
        std::map<std::string,pixel> table;
    public:
        colors(){return;};
        void add_color(std::string color,pixel rgb);
        void add_color(std::string color,uchar r, uchar g, uchar b);
        pixel rgb(std::string color);
        uchar red(std::string color);
        uchar green(std::string color);
        uchar blue(std::string color);
};

void colors::add_color(std::string color,pixel rgb)
{
    table[color] = rgb;
}
void colors::add_color(std::string color,uchar r, uchar g, uchar b)
{
    pixel p(r,g,b);
    table[color] = p;
}

pixel colors::rgb(std::string color)
{
    return table[color];
}

uchar colors::red(std::string color)
{
    return table[color].red();
}
uchar colors::green(std::string color)
{
    return table[color].green();
}

uchar colors::blue(std::string color)
{
    return table[color].blue();
}

*/

class hash_color
{
    private:
        pixel values[30];
        int lenght;
    public:
        hash_color(){return;};
        int getKey(char* key);
        void insert(char *key,uchar r, uchar g, uchar b);
        uchar red(char* key);
        uchar green(char* key);
        uchar blue(char* key);
};


int hash_color::getKey(char* key)
{
    int value=0;
    char *c=key;
    while(*c!='\0')
    {
        value += *c;
        c++;
    }
    value %= 30;
    return value; 
}

void hash_color::insert(char *key,uchar r, uchar g, uchar b)
{
    int value =  this->getKey(key);
    pixel p(r,g,b);
    values[value] = p;
}

uchar hash_color::red(char* key)
{
    int value =  this->getKey(key);
    return values[value].red(); 
}
uchar hash_color::green(char* key)
{
    int value =  this->getKey(key);
    return values[value].green(); 
}
uchar hash_color::blue(char* key)
{
    int value =  this->getKey(key);
    return values[value].blue(); 
}



int main()
{
    
    hash_color colors;
    colors.insert((char*)("blue"),0,0,255);
    colors.insert((char*)("red"),255,0,0);
    colors.insert((char*)("green"),255,0,0);
    std::cout<<int(colors.blue((char*)("blue")));
    return 0;
}