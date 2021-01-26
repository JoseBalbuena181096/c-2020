//**************** CLASS OF PIXELS *********************************
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
//***************** CLASS OF HASH MAP COLORS ************************************
class hash_color
{
    private:
        pixel values[100];
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
    value %= 100;
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
//******************** CLASS LEDS ***********************************************
class LedRGB
{
    private:
        hash_color list; 
        int pin_r,pin_g,pin_b;
        
    public:
        LedRGB(int pin_r,int pin_g,int pin_b);
        void set_color(char* color);

};

LedRGB::LedRGB(int pin_r,int pin_g,int pin_b)
{
    this->pin_r = pin_r;
    this->pin_g = pin_g;
    this->pin_b = pin_b;
    list.insert( (char*)("red"),255,0,0); 
    list.insert((char*)("maroon"),128,0,0);  
    list.insert((char*)("yellow"),255,255,0);    
    list.insert((char*)("olive"),128,128,0);
    list.insert((char*)("lime"),0,255,0); 
    list.insert((char*)("green"),0,128,0);
    list.insert((char*)("aqua"),0,255,255);
    list.insert((char*)("teal"),0,128,128);
    list.insert((char*)("blue"),0,0,255);
    list.insert((char*)("navy"),0,0,128);
    list.insert((char*)("fuchsia"),255,0,255);
    list.insert((char*)("purple"),25,0,255);
    list.insert((char*)("white"),200,200,255);
    list.insert((char*)("black"),0,0,0);
    list.insert((char*)("orange"),255,104,15);
}

void LedRGB::set_color(char* color)
{
  analogWrite(pin_r,list.red(color));  
  analogWrite(pin_g,list.green(color));  
  analogWrite(pin_b,list.blue(color)); 
}



char DataBluetooth = ' ';

bool mode = false;
int i=0;
/*
char *list_colors[12]= {(char*)("red"),(char*)("maroon"),(char*)("yellow"),(char*)("olive"),
                          (char*)("lime"),(char*)("green"),(char*)("aqua"),(char*)("teal"),
                          (char*)("blue"),(char*)("navy"),(char*)("fuchsia"),(char*)("purple")};
char **p = list_colors;
*/

LedRGB RGB_1(3,4,2);
LedRGB RGB_2(6,7,5);
LedRGB RGB_3(9,10,8);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  
}
void loop() {
  
        RGB_1.set_color((char*)("white"));
        RGB_2.set_color((char*)("white")); 
        RGB_3.set_color((char*)("white")); 
        
}


void serialEvent(){

if(Serial.available())
    {
      DataBluetooth = Serial.read();

      if(DataBluetooth == 'A')//12
      {
        mode = true;     
      }
      else if(DataBluetooth == 'M')//12
      {
        mode = false;      
      }       
    }

}
