 #define pin_red 5
#define pin_green 3
#define pin_blue 6

char DataBluetooth = ' ';

bool mode = false;
char *list_colors[12]= {(char*)("red"),(char*)("maroon"),(char*)("yellow"),(char*)("olive"),
                          (char*)("lime"),(char*)("green"),(char*)("aqua"),(char*)("teal"),
                          (char*)("blue"),(char*)("navy"),(char*)("fuchsia"),(char*)("purple")};
char **p = list_colors;
int i=0;



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

class colors
{
    private:
        hash_color list; 
    public:
        colors();
        void set_color(char* color);

};

colors::colors()
{
    list.insert((char*)("red"),255,0,0); 
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
    list.insert((char*)("purple"),128,0,128);
    
}

void colors::set_color(char* color)
{
  analogWrite(pin_red,list.red(color));  
  analogWrite(pin_green,list.green(color));  
  analogWrite(pin_blue,list.blue(color)); 
}

colors RGB;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  
}
void loop() {
  
  ////////////////////////////R,G,B
    
  // put your main code here, to run repeatedly:
  
if(mode){
  if(DataBluetooth == '1')
  {
    i=0;
    while(i<12)
   {
       RGB.set_color(p[i++]);
       delay(50);
    }    
  }
  else if(DataBluetooth == '2')
  {
    i=0;
    while(i<12)
   {
       RGB.set_color(p[i++]);
       delay(500);
    }    
  }
  else if(DataBluetooth == '3')
  {
    i=11;
    while(i>=0)
   {
       RGB.set_color(p[i--]);
       delay(50);
    }    
  }
  else if(DataBluetooth == '4')
  {
    i=11;
    while(i>=0)
   {
       RGB.set_color(p[i--]);
       delay(500);
    }   
  }
    else if(DataBluetooth == '5')
  {
 for(int i=0;i<20;i++){
    RGB.set_color((char*)("maroon"));
    delay(50);
    RGB.set_color((char*)("fuchsia"));
    delay(50);    
    }
  for(int i=0;i<20;i++){
    RGB.set_color((char*)("purple"));
    delay(50);
    RGB.set_color((char*)("blue"));
    delay(50);    
    }
  }
    else if(DataBluetooth == '6')
  {
 for(int i=0;i<20;i++){
    RGB.set_color((char*)("maroon"));
    delay(500);
    RGB.set_color((char*)("fuchsia"));
    delay(500);    
    }
  for(int i=0;i<20;i++){
    RGB.set_color((char*)("purple"));
    delay(500);
    RGB.set_color((char*)("blue"));
    delay(500);    
    }
  }
  
    else if(DataBluetooth == '7')
  {
    RGB.set_color((char*)("red"));
    delay(50);
    RGB.set_color((char*)("yellow"));
    delay(50);    
    RGB.set_color((char*)("lime"));
    delay(50);   
  }
    else if(DataBluetooth == '8')
  {
    RGB.set_color((char*)("red"));
    delay(500);
    RGB.set_color((char*)("yellow"));
    delay(500);    
    RGB.set_color((char*)("lime"));
    delay(500);   
  }
  
  
} else{
      if(DataBluetooth == 'a')//1
      {
       RGB.set_color((char*)("red"));    
      }
      else if(DataBluetooth == 'b')//2
      {
        RGB.set_color((char*)("maroon"));   
      }
      else  if(DataBluetooth == 'c')//3
      {
        RGB.set_color((char*)("yellow"));        
      }      
      else  if(DataBluetooth == 'd')//4
      {
 
        RGB.set_color((char*)("olive"));       
      }
      else if(DataBluetooth == 'f')//5
      {
 
       RGB.set_color((char*)("lime"));       
      }
      else if(DataBluetooth == 'g')//6
      {
        RGB.set_color((char*)("green"));        
      }
      else if(DataBluetooth == 'h')//7
      {
        RGB.set_color((char*)("aqua")); 
      }
      else if(DataBluetooth == 'i')//8
      {
 
        RGB.set_color((char*)("teal"));     
      }
      else if(DataBluetooth == 'j')//9
      {
        RGB.set_color((char*)("blue"));      
      }
      else if(DataBluetooth == 'k')//10
      {
         RGB.set_color((char*)("navy")); 
      }
      else  if(DataBluetooth == 'p')//11
      {
 
        RGB.set_color((char*)("fuchsia"));      
      }
      else if(DataBluetooth == 'q')//12
      {
 
        RGB.set_color((char*)("purple"));      
      }

      }
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
