#ifndef SIMPLESDL_H
#define SIMPLESDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

/**
 * Classe che wrappa alcune funzionalit√† della libreria [SDL2](http://www.libsdl.org).
 *
 * Utilizzando SimpleSDL √® possibile utilizzare le funzionalit√† grafiche
 * (gestione colori, primitive base, font) e catturare i principali eventi
 * (mouse,tastiera) presenti nella libreria SDL2.
 *
 * @author Marco Vassura
 * @version 1.1
 */
class SimpleSDL {

protected:
    int screen_w;
    int screen_h;
    SDL_Window* wnd;
    SDL_Renderer* rnd;
    SDL_Event event;
    bool hasEvent;
    std::map<std::string,SDL_Color> colors;
    std::map<std::string,SDL_Texture*> images;

    TTF_Font* font;
    int fontSize;

public:
    /**
     * Inizializza le componenti SDL e crea una finestra grafica.
     *
     * Questo costruttore effettua le operazioni di inizializzazione e creazione della finestra grafica nel
     * seguente ordine:
     * - inizializza sottosistema video di SDL;
     * - crea la finestra grafica;
     * - imposta i colori predefiniti "bg" e "fg" rispettivamente a nero e bianco;
     * - inizializza sottosistema truetype di SDL;
     * - imposta il font a DejaVuSansMono e la dimensione a 12 punti;
     * - imposta il colore iniziale a "fg".
     * - imposta lo sfondo dello schermo al colore "bg".
     *
     * @param title il titolo della finestra.
     * @param x la coordinata x in pixel dell'angolo in alto a sinistra della finestra.
     * @param y la coordinata y in pixel dell'angolo in alto a sinistra della finestra.
     * @param w la larghezza in pixel della finestra.
     * @param h l'altezza in in pixel della finestra.
     * @exception SimpleSDLException se non √® possibile creare la finestra grafica.
     */
    SimpleSDL(const char* title, int x, int y, int w, int h);

    /**
     * Inizializza le componenti SDL e crea una finestra grafica con origine (0,0).
     *
     * @param title il titolo della finestra.
     * @param w la larghezza in pixel della finestra.
     * @param h l'altezza in in pixel della finestra.
     * @exception SimpleSDLException se non √® possibile creare la finestra grafica.
     */
    SimpleSDL(const char* title, int w, int h): SimpleSDL(title,-1,-1,w,h) {}

    /**
     * Definisce un colore.
     *
     * Permette di associare ad un nome un colore con le componenti r,g,b.
     * successivamente √® possibile utilizzare il nome come riferimento.
     * Nel caso esista gi√† un colore con lo stesso nome sar√† sostituito.
     *
     * @param name il nome del colore.
     * @param r il byte con la componente rosso del colore.
     * @param g il byte con la componente verde del colore.
     * @param b il byte con la componente blue del colore.
     */
    int setColor(const std::string name,Uint8 r,Uint8 g,Uint8 b);

    /**
     * Imposta il colore corrente.
     *
     * Permette di definire quale colore sar√† utilizzato per il disegno delle
     * successive primitive grafiche.
     *
     * @param name il nome del colore.
     * @return 0 se il colore esiste, 1 se il colore non esiste.
     */
    int useColor(const std::string name);

    /**
     * Imposta il colore corrente al colore di sfondo.
     */
    void useColorBg()
    {
        useColor("bg");
    }

    /**
     * Imposta il colore corrente al colore base.
     */
    void useColorFg()
    {
        useColor("fg");
    }

    /**
     * Cancella lo schermo.
     *
     * @return 0 se Ë tutto andato bene, <0 se qualcosa Ë andato storto.
     */
    int clear();

    /**
     * Aggiorna la finestra grafica con gli oggetti disegnati dall'ultimo redraw.
     */
    void redraw();

    /**
	 * Disegna punto.
	 *
	 * Permette di disegnare un punto sullo schermo indicando le sue coordinate.
	 * 
	 * @param x la coordinata delle x del punto.
	 * @param y la coordinata delle y del punto.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 */
    int drawPoint(int x, int y);
    
	/**
	 * Disegna un segmento.
	 *
	 * Permette di disegnare un segmento indicando le coordinate di partenza dell'origine
	 * e le coordinate di arrivo del segmento.
	 *
	 * @param x1 la coordinata x dell'origine.
	 * @param y1 la coordinata y dell'origine.
	 * @param x2 la coordinata x del punto d'arrivo.
     * @param y2 la coordinata y del punto d'arrivo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore. 
	 */
    int drawLine(int x1, int y1, int x2, int y2);
    
	/**
	 * Disegna rettangolo .
	 * 
	 * Disegna un rettangolo disegnando delle linee che collegano i 4
	 * vertici, che ha coma vertice in altro a sinistra delle coordinate sullo schermo
	 * e come altezza e larghezza un numero di pixel inseriti.
	 *
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 */
    int drawRect(int x1, int y1, int w, int h);
    
    /**
	 * Disegna ovale.
	 * Disegna un ovale inscritto in un rettangolo del quale Ë necessario
	 * definire la coordinate del vertice in alto a sinistra, la larghezza e l'altezza.
	 * 
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */    
	int drawOval(int x1, int y1, int w, int h);

	/**
	 * Disegna rettangolo colorato.
	 *
	 * Disegna un rettangolo che viene completamente colorato internamente
	 * con il colore selezionato al momento del call della funzione
	 * e applica lo stesso funzionamento di drawRect ovvero inserendo coordinate di 
	 * un vertice e aggiungendo poi la larghezza e l'altezza.
	 *
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 */    
	int fillRect(int x1, int y1, int w, int h);
    
    /**
	 * Disegna un ovale colorato 
	 *
	 * Disegna un ovale che viene completamente colorato internamente
	 * con il colore selezionato al momento del call della funzione
	 * e applica lo stesso funzionamento di drawOval ovvero inserendo coordinate di 
	 * un vertice e aggiungendo poi la larghezza e l'altezza del rettangolo all'interno del quale 
	 * verr‡ disegnato l'ovale.
	 *
	 * @param x1 coordinata x del vertice in alto a sinistra.
	 * @param y1 coordinata y del vertice in alto a sinistra.
	 * @param w la larghezza in pixel del rettangolo.
	 * @param h l'altezza in pixel del rettangolo.
	 * 
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
	int fillOval(int x1, int y1, int w, int h);
	
	/**
	 * Verifica l'avvenimento di un evento di input di qualsiasi tipo.
	 *
	 * @return 1 nel caso in cui sia accaduto un evento.
	 * @return 0 nel caso in cui non sia accaduto un evento.
	 */
    bool checkEvent();

	/**
	 * Verifica che l'azione compiuta sia un movimento del mouse.
	 *
	 * @return TRUE se c'Ë stato un movimento del mouse, FALSE se non c'Ë stato.
	 */
    bool isMouseMotionEvent();
    
	/**
	 * Verifica che l'azione compiuta sia un click del mouse.
	 *
	 * @return TRUE se c'e stato un click del mouse, FALSE se non c'Ë stato.
	 */
	bool isMouseButtonEvent();
	
	/**
	 * verifica che l'azione sia la pressione di un tasto.
	 *
	 * @return TRUE se c'Ë stato un'abbassamento del tasto, FALSE se non c'Ë stato.
	 */
    bool isKeyEvent();

  	/**
  	 * Cattura la coordinata X del click del mouse.
  	 *
  	 * @return la coordinata X del punto in cui Ë stato cliccato il mouse, -1 se non c'Ë stato nessun evento o se non era un click del mouse.
  	 */
    Sint32 getMouseX();
    
 	/**
  	 * Cattura la coordinata Y del click del mouse.
  	 *
  	 * @return la coordinata Y del punto in cui Ë stato cliccato il mouse, -1 se non c'Ë stato nessun evento o se non era un click del mouse.
  	 */
    Sint32 getMouseY();
    
	/**
	 * Indica quale tasto del mouse Ë stato cliccato.
	 *
	 * @return 1 se era il tasto sinistro.
	 * @return 2 se era il tasto centrale.
	 * @return 3 se era il tasto destro.
	 * @return 0 se non era nË destro, nË sinistro, nË centrale.
	 * @return -1 se non c'Ë stato nessun click del mouse.
	 */
	int getMouseButton();
	
	/**
	* Conta il numero di click del mouse.
	*
	* @return il numero di click del mouse.
	* @return 0 se non ci sono stati click del mouse.
	*/
    int getMouseButtonCount();

	/**
	 * Restituisce il tasto premuto dall'utente.
	 *
	 * @return SDLK_a se il tasto premuto altrimenti dopo SDLK_ c'Ë il tasto premuto (https://wiki.libsdl.org/SDL_Keycode)
	 * @return 0 se non Ë stato premuto nessun tasto.
	 */
    SDL_Keycode getKeyCode();

	/**
	 * Restituisce la larghezza dello schermo.
	 *
	 * @return la larghezza dello schermo in pixel.
	 *
	 */
    int width()
    {
        return screen_w;
    }
   
    /**
	 * Restituisce l'altezza dello schermo.
	 *
	 * @return restituisce l'altezza dello schermo in altezza.
	 *
	 */   
    int height()
    {
        return screen_h;
    }

	/**
	 * Imposta delay.
	 * 
	 * Blocca il programma per una durata di millisecondi e poi lo fa riprendere.
	 *
	 * @param ms il numero di millesecondi per il quale il programma si deve mettere in pausa.
	 * 
	 */
    void delay(int ms);

	/**
	 * Svuota buffer input.
	 * 
	 * Svuota il buffer che registra il movimento della rotella del mouse
	 * e quello che registra i tasti premuti.
	 *
	 */
	void flushEvents();

	/**
	 * Stampa stringa in un punto dello schermo con una grandezza del carattere definito.
	 *
	 * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare e definendo la grandezza del carattere.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param std::string msg la stringa che deve essere stampata .
	 * @param size la grandezza del carattere del testo.
	 *
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
	 * 
	 */
    int text(int x, int y, const std::string msg, int size);
    
    /**
     * Stampa stringa in un punto dello schermo.
     *
     * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param std::string msg la stringa che deve essere stampata .
	 *
	 * @return -1 nel caso in cui non ci sia nessun font selezionato.
	 * @return 0 se Ë andato tutto bene.
	 *
     */
    int text(int x, int y, const std::string msg);
    
    /**
     * Stampa un a stringa di caratteri di grandezza definita al centro dello schermo.
     *
     * @param std::string msg indica la stringa che verr‡ stampata.
     * @param size indica la grandezza dei caratteri.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const std::string msg, int size);
    
    /**
     * Stampa una stringa nel centro dello schermo.
     *
     * @param std::string msg indica la stringa da stampare.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const std::string msg);

	/**
	 * Stampa stringa in un punto dello schermo con una grandezza del carattere definito.
	 *
	 * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare e definendo la grandezza del carattere.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param std::string msg la stringa che deve essere stampata .
	 * @param size la grandezza del carattere del testo.
	 *
	 * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore. 
	 */
    int text(int x, int y, const char* msg, int size);
    
	/**
     * Stampa stringa in un punto dello schermo.
     *
     * Stampa una stringa indicando le coordinate del punto pi˘ in alto e a sinistra della stringa
	 * inserendo poi una stringa di char da stampare.
	 *
	 * @param x coordinata x di inizio della stringa.
	 * @param y coordinata y di inizio della stringa.
	 * @param msg il messaggio che deve essere stampato.
	 *
	 * @return -1 nel caso in cui non ci sia nessun font selezionato, 0 se tutto Ë andato bene.
     */
    int text(int x, int y, const char* msg);
    
    /**
     * Stampa un a stringa di caratteri di grandezza definita al centro dello schermo.
     *
     * @param msg il messaggio che deve essere stampato.
     * @param size indica la grandezza dei caratteri.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const char* msg, int size);
    
    /**
     * Stampa una stringa nel centro dello schermo.
     *
     * @param msg il messaggio che deve essere stampato.
     *
     * @return 0 se Ë andato tutto bene, se !=0 c'Ë stato qualche errore.
     */
    int text(const char* msg);

	/**
	 * Per indicare la grandezza di default del font.
	 *
	 * @param size indica la grandezza del font.
	 *
	 * @return -1 la grandezza del font Ë <=0 o >della grandezza dello schermo.
	 */
    int textSize(int size);
    
    /**
     * Restituisce la grandezza del font di default al momento.
     *
     * @return grandezza del font del testo
     */
    int getTextSize();

    int setImage(const std::string name, const std::string filename);
    
	int drawImage(int x, int y, const std::string name);
	
	int drawTLine(int x1, int y1, int x2, int y2, int t);
	

    ~SimpleSDL();
};

#endif // SIMPLESDL_H
