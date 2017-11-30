#include "SimpleSDL.h"
#include "SimpleSDLException.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <stdio.h>

SimpleSDL::SimpleSDL(const char* title, int x, int y, int w, int h)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw SimpleSDLException(SDL_GetError());
    }
 	if (x==-1 && y==-1) {
    	SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
    		x=10;
    		y=10;
    	} else {
    		x = (dm.w-w)/2;
    		y = (dm.h-h)/2;
		}
	}

    atexit(SDL_Quit);

    wnd = SDL_CreateWindow(title, x, y, w, h, 0);
    if (wnd == NULL)
    {
        throw SimpleSDLException(SDL_GetError());
    }
    SDL_GetWindowSize(wnd,&screen_w,&screen_h);
    rnd = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    if (rnd == NULL)
    {
        throw SimpleSDLException(SDL_GetError());
    }

    setColor("bg",0,0,0);
    setColor("fg",255,255,255);
    useColor("fg");

    if (TTF_Init()!=0)
    {
        throw SimpleSDLException(TTF_GetError());
    }
    font = NULL;
    fontSize = 12;
    textSize(fontSize);
    if (font == NULL)
    {
        throw SimpleSDLException(TTF_GetError());
    }

    hasEvent = false;

    clear();
    redraw();
}


int SimpleSDL::clear()
{
    return SDL_RenderClear(rnd);
}

void SimpleSDL::redraw()
{
    SDL_RenderPresent(rnd);
}

int SimpleSDL::setColor(const std::string name, Uint8 r,Uint8 g,Uint8 b)
{
    try
    {
        SDL_Color c = {r,g,b,SDL_ALPHA_OPAQUE};
        colors[name] = c;
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
    return 0;
}

int SimpleSDL::useColor(const std::string name)
{
    try
    {
        SDL_Color c = colors.at(name);
        return SDL_SetRenderDrawColor(rnd,c.r, c.g, c.b, c.a);
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
    return 0;
}



	
int SimpleSDL::drawLine(int x1, int y1, int x2, int y2)                                                        
{																											   
    return SDL_RenderDrawLine(rnd,x1,y1,x2,y2);
					 
}			  																						  
																											 
int SimpleSDL::drawRect(int x1, int y1, int w, int h)														  
{																											   
    SDL_Rect r = {x1,y1,w,h};																				  
    return SDL_RenderDrawRect(rnd, &r);
}

int SimpleSDL::drawOval(int x1, int y1, int w, int h)
{
    float theta, angle_increment;
    float w2 = w/2;
    float h2 = h/2;
    int xc = x1+w2;
    int yc = y1+h2;
    int n=10;

    SDL_Point points[4*n];

    angle_increment = M_PI_2 / n;

    int i=0;
    for (theta = 0.0f; theta <= M_PI_2; theta += angle_increment)
    {
        points[i].x = w2 * SDL_cos(theta);
        points[i].y = h2 * SDL_sin(theta);
        points[2*n-i-1].x = xc - points[i].x;
        points[2*n-i-1].y = yc + points[i].y;
        points[2*n+i].x = xc - points[i].x;
        points[2*n+i].y = yc - points[i].y;
        points[4*n-i-1].x = xc + points[i].x;
        points[4*n-i-1].y = yc - points[i].y;
        points[i].x += xc;
        points[i].y += yc;
        i++;
    }
    return SDL_RenderDrawLines(rnd,points,4*n);
}

int SimpleSDL::drawPoint(int x, int y)
{
    return SDL_RenderDrawPoint(rnd, x,y);
}

int SimpleSDL::fillRect(int x1, int y1, int w, int h)
{
    SDL_Rect r = {x1,y1,w,h};
    return SDL_RenderFillRect(rnd, &r);
}

int SimpleSDL::fillOval(int x1, int y1, int w, int h)
{
    float a = w/2;
    float b = h/2;
    int xc = x1+a;
    int yc = y1+b;
    float a2 = a*a;
    float b2 = b*b;
    float dq;
    if (a<b)
    {
        for(float x=0; x<=a; x++)
        {
            dq = b*SDL_sqrt(a2-x*x)/a;
            drawLine(xc+x,yc-dq,xc+x,yc+dq);
            drawLine(xc-x,yc-dq,xc-x,yc+dq);
        }
    }
    else
    {
        for(int y=0; y<=b; y++)
        {
            dq = a*SDL_sqrt(b2-y*y)/b;
            drawLine(xc-dq,yc+y,xc+dq,yc+y);
            drawLine(xc-dq,yc-y,xc+dq,yc-y);
        }
    }
    return 0;
}

bool SimpleSDL::checkEvent()
{
    hasEvent = (SDL_PollEvent(&event) == 1);
    return hasEvent;
}


bool SimpleSDL::isMouseMotionEvent()
{
    return (hasEvent && event.type == SDL_MOUSEMOTION);
}

bool SimpleSDL::isMouseButtonEvent()
{
    return (hasEvent && (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP));
}

bool SimpleSDL::isKeyEvent()
{
    return (hasEvent && event.type == SDL_KEYDOWN);
}

Sint32 SimpleSDL::getMouseX()
{
    if (isMouseMotionEvent()||isMouseButtonEvent())
    {
        return event.motion.x;
    }
    else
    {
        return -1;
    }
}

Sint32 SimpleSDL::getMouseY()
{
    if (isMouseMotionEvent()||isMouseButtonEvent())
    {
        return event.motion.y;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::getMouseButton()
{
    if (isMouseButtonEvent())
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            return 1;
        case SDL_BUTTON_MIDDLE:
            return 2;
        case SDL_BUTTON_RIGHT:
            return 3;
        }
        return 0;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::getMouseButtonCount()
{
    if (isMouseButtonEvent())
    {
        return event.button.clicks;
    }
    else
    {
        return 0;
    }
}

SDL_Keycode SimpleSDL::getKeyCode()
{
    if (isKeyEvent())
    {
        return event.key.keysym.sym;
    }
    else
    {
        return 0;
    }
}

void SimpleSDL::delay(int ms)
{
    SDL_Delay(ms);
}

void SimpleSDL::flushEvents()
{
	SDL_FlushEvents(SDL_KEYDOWN, SDL_MOUSEWHEEL);
}

int SimpleSDL::textSize(int size)
{
    if (size<=0 || size>screen_w)
    {
        return -1;
    }
    if (font != NULL && size==TTF_FontHeight(font))
        return 0;
    if (font != NULL)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
    font = TTF_OpenFont("DejaVuSansMono.ttf", size); //this opens a font style and sets a size
    if (font != NULL )
    {
        fontSize = size;
        return 0;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::getTextSize()
{
    return fontSize;
}

int SimpleSDL::text(int x, int y, const std::string msg)
{
    return text(x,y, msg.c_str());
}

int SimpleSDL::text(int x, int y, const char* msg)
{
    if (font!=NULL)
    {
        SDL_Color c;
        SDL_GetRenderDrawColor(rnd,&c.r, &c.g, &c.b, &c.a);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, msg, c);
        SDL_Texture* text = SDL_CreateTextureFromSurface(rnd, textSurface);
        int text_width = textSurface->w;
        int text_height = textSurface->h;
        SDL_FreeSurface(textSurface);
        SDL_Rect renderQuad = { x, y, text_width, text_height };
        SDL_RenderCopy(rnd, text, NULL, &renderQuad);
        SDL_DestroyTexture(text);
        return 0;
    }
    else
    {
        return -1;
    }
}

int SimpleSDL::text(int x, int y, const std::string msg, int size)
{
    return text(x,y,msg.c_str(),size);
}

int SimpleSDL::text(int x, int y, const char* msg, int size)
{
    textSize(size);
    return text(x,y,msg);
}


int SimpleSDL::text(const std::string msg, int size)
{
    return text(msg.c_str(), size);
}

int SimpleSDL::text(const char* msg, int size)
{
    textSize(size);
    return text(msg);
}


int SimpleSDL::text(const std::string msg)
{
    return text(msg.c_str());
}

int SimpleSDL::text(const char* msg)
{
    int w,h;
    TTF_SizeText(font,msg,&w,&h);
    return text((screen_w-w)/2,(screen_h-h)/2,msg);
}

int SimpleSDL::setImage(const std::string name, const std::string filename)
{
    try
    {
        SDL_Surface* img = SDL_LoadBMP(filename.c_str());
        if (img != NULL)
        {
            SDL_Color bg = colors["bg"];
            SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format,bg.r,bg.g,bg.b));
            images[name] = SDL_CreateTextureFromSurface(rnd, img);
            SDL_FreeSurface(img);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
}

int SimpleSDL::drawImage(int x, int y, const std::string name)
{
    try
    {
        SDL_Texture* img = images.at(name);
        int w,h;
        SDL_QueryTexture(img, NULL, NULL, &w, &h);
        SDL_Rect dstRect = {x,y,w,h};
        SDL_RenderCopy(rnd, img, NULL, &dstRect);
        return 0;
    }
    catch (const std::out_of_range& oor)
    {
        return 1;
    }
}

int SimpleSDL::drawTLine(int x1, int y1, int x2, int y2, int t)                                                        
{	
	if(t>0){
			if(t%2!=0){t++;}
			//x1=x1-(t/2);
			y1=y1-(t/2);
			//x2=x2+(t/2);
			y2=y2-(t/2);
			for( ; t!=0; t--){
							
						      drawLine(x1,y1,x2,y2);
							  //x1++;
							  y1++;
							  //x2++;
							  y2++;}
			return 0;}
			else{
				  return -1;
				  }
}		


SimpleSDL::~SimpleSDL()
{
    for (std::map<std::string,SDL_Texture*>::iterator img=images.begin(); img!=images.end(); ++img)
    {
        SDL_DestroyTexture(img->second);
    }
    SDL_DestroyRenderer(rnd);
    SDL_DestroyWindow(wnd);
    SDL_Quit();
    if (TTF_WasInit())
    {
        if (font != NULL)
        {
            TTF_CloseFont(font);
        }
        TTF_Quit();
    }
}



