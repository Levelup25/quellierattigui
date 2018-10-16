#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>

//#include "state.h"

using namespace std;
//using namespace state;

#include "string.h"

int main(int argc,char* argv[]) 
{
    int n=4,m=4,N=n*n,M=m*m,l=36,h=25,l2=25,h2=32;
    // create the window
    sf::RenderWindow window(sf::VideoMode(N*l,M*h),"Jeu");
    window.setKeyRepeatEnabled(false);
    
    int seed=time(NULL);
    //scanf ("%d",&seed);
    srand(seed);
    sf::Texture t_neutre,t_eau,t_terre,t_feu,t_air,t_perso;
    t_neutre.loadFromFile("/home/ensea/quellierattigui/src/client/tileset/isometric_pixel_flat_0000.png", sf::IntRect(0, 0, l, h)); //0
    t_eau.loadFromFile("/home/ensea/quellierattigui/src/client/tileset/isometric_pixel_flat_0028.png", sf::IntRect(0, 0, l, h));    //1
    t_terre.loadFromFile("/home/ensea/quellierattigui/src/client/tileset/isometric_pixel_flat_0055.png", sf::IntRect(0, 0, l, h));  //2
    t_feu.loadFromFile("/home/ensea/quellierattigui/src/client/tileset/isometric_pixel_flat_0037.png", sf::IntRect(0, 0, l, h));    //3
    t_air.loadFromFile("/home/ensea/quellierattigui/src/client/tileset/isometric_pixel_flat_0093.png", sf::IntRect(0, 0, l, h));    //4
    t_perso.loadFromFile("home/ensea/quellierattigui/src/client/persos.png");
    
    sf::Sprite neutre,eau,terre,feu,air,perso,sprite;
    neutre.setTexture(t_neutre);
    eau.setTexture(t_eau);
    terre.setTexture(t_terre);
    feu.setTexture(t_feu);
    air.setTexture(t_air);
    perso.setTexture(t_perso);
    sf::IntRect rect(0, 0, l2, h2);
    perso.setTextureRect(rect);
    perso.setScale(sf::Vector2f(1,(float)h/h2));
    sf::Clock clock;
    
    vector<int> vector {4,3,2,1};
    random_shuffle(vector.begin(),vector.end());
    vector.push_back(0);
    
    int r,p=5;
    
    int map[N][M];
    for (int j=0;j<M;j++) {
        for (int i=0;i<N;i++) {
            r=rand() % 100;
            for (int k=0;k<(int)vector.size();k++) {
                if (r>= k*p && r < (k+1)*p) {
                    map[i][j]=k;
                }
            }
            if (r>=(int)vector.size()*p) {
                if(i>=N/4 && i<3*N/4 && j>=M/4 && j<3*M/4) {
                    map[i][j]=0;
                }
                else if(i<N/2 && j<M/2) {
                    map[i][j]=1;
                }
                else if(i<N/2 && j>=M/2) {
                    map[i][j]=2;
                }
                else if(i>=N/2 && j<M/2) {
                    map[i][j]=3;
                }
                else {
                    map[i][j]=4;
                }
            }
        }
    }
    
    int xp,yp,xc,yc;
    perso.setPosition(sf::Vector2f(2*n*l+(l-l2)/2,2*m*h));
    window.draw(perso);
    xp=perso.getPosition().x;
    yp=perso.getPosition().y;
    
    xc=(int)(xp/l/n)*n+n/2;
    yc=(int)(yp/h/m)*m+m/2;
    sf::View view(sf::Vector2f(xc*l,yc*h),sf::Vector2f(n*l,m*h));
    window.setView(view);
//    sf::View minimap;
//    minimap.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
//    window.setView(minimap);
    
    int s=1;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        for (int j=yc-m/2;j<yc+m/2;j++) {
            for (int i=xc-n/2;i<xc+n/2;i++) {
                if(map[i][j]==0) {
                    sprite=neutre;
                }
                else if(map[i][j]==1) {
                    sprite=eau;
                }
                else if(map[i][j]==2) {
                    sprite=terre;
                }
                else if(map[i][j]==3) {
                    sprite=feu;
                }
                else if(map[i][j]==4) {
                    sprite=air;
                }
                sprite.setPosition(sf::Vector2f(i*l,j*h));
                window.draw(sprite);
            }
        }
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left && xp>=l)
                {
                    perso.move(-l,0);
                    rect.top=h2;
                    perso.setTextureRect(rect);
                }
                if (event.key.code == sf::Keyboard::Right && xp<(N-1)*l)
                {
                    perso.move(l,0);
                    rect.top=2*h2;
                    perso.setTextureRect(rect);
                }
                if (event.key.code == sf::Keyboard::Up && yp>=h)
                {
                    perso.move(0,-h);
                    rect.top=3*h2;
                    perso.setTextureRect(rect);
                }
                if (event.key.code == sf::Keyboard::Down && yp<(M-1)*h)
                {
                    perso.move(0,h);
                    rect.top=0;
                    perso.setTextureRect(rect);
                }
                window.draw(perso);
                xp=perso.getPosition().x;
                yp=perso.getPosition().y;
                xc=(int)(xp/l/n)*n+n/2;
                yc=(int)(yp/h/m)*m+m/2;
                view.setCenter(sf::Vector2f(xc*l,yc*h));
                window.setView(view);
            }
            
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (clock.getElapsedTime().asSeconds() > 0.5f){
            if (rect.left == 0)
                s=1;
            else if (rect.left == 2*h2)
                s=-1;
            rect.left += h2*s;

            perso.setTextureRect(rect);
            clock.restart();
        }
        
        window.draw(perso);
        // clear the window with black color
        //window.clear(sf::Color::Black);
        
        // end the current frame
        window.display();
    }
    
    for (int i=1;i<argc;i++) {
        // Livrable 1.1
        if (strcmp(argv[i],"hello")==0) { 
            cout << "Bonjour le monde !" << endl;
        }

        // Livrable 1.final
        else if (strcmp(argv[i],"state")==0) { 

        }

        // Livrable 2.1
        else if (strcmp(argv[i],"render")==0) { 

        }

        // Livrable 2.2
        else if (strcmp(argv[i],"engine")==0) { 

        }

        // Livrable 2.final
        else if (strcmp(argv[i],"random_ai")==0) { 

        }

        // Livrable 3.1
        else if (strcmp(argv[i],"heuristic_ai")==0) { 

        }

        // Livrable 3.final
        else if (strcmp(argv[i],"rollback")==0) { 

        }

        else if (strcmp(argv[i],"deep_ai")==0) { 

        }

        // Livrable 4.1
        else if (strcmp(argv[i],"thread")==0) { 

        }

        else if (strcmp(argv[i],"record")==0) { 

        }

        else if (strcmp(argv[i],"play")==0) { 

        }

        // Livrables 4.2 et 4.final
        else if (strcmp(argv[i],"listen")==0) { 

        }

        else if (strcmp(argv[i],"network")==0) { 

        }
    }
    return 0;
}
