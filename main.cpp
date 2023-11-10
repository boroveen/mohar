#include <filesystem>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ncurses.h>
using namespace std;
namespace fs = filesystem;
bool visual=false,matem=true,onematem=false,btitle=false;
int row,cal,arow,acal,iflag,iinit=0,ifile=1,iword=0,iline[1000],allline=2,ititle=0;
string sdir[100],spwd,sword[100000];

int onenumber=27,twonumber=18;
bool boolnumber=true;

/*
ifile - текстовый документ
iword - сколько слов
*/

void checksmallscreen(){
	getmaxyx(stdscr,row,cal);
	if(row<=22 || cal<=100){
		bool gg=true;
		while(gg){
			clear();
			attron(COLOR_PAIR(3));
			printw("маленький размер экрана");
			switch(getch()){
				case 410:
					getmaxyx(stdscr,row,cal);
					if(row>=22 && cal>=100) gg=false;
				break;
				case 'q': visual=false,matem=false,gg=false; break;
				default: break;
			}
		}
	}
}

void vhelp(){
	bool vv=true,vvv=true;
	while(vv){
		clear();
		getmaxyx(stdscr,row,cal);

		attron(COLOR_PAIR(1));
		mvprintw(0,cal/2/2,"┏");
		for(int i=0;i<cal/2-1;i++) printw("━");
		printw("┓");
		for(int i=1;i<row-1;i++) mvprintw(i,cal/2/2,"┃");
		for(int i=1;i<row-1;i++) mvprintw(i,cal/2+cal/2/2,"┃");
		mvprintw(row-1,cal/2/2,"┗");
		for(int i=0;i<cal/2-1;i++) printw("━");
		printw("┛");

		if(visual){
			attron(A_BLINK);
			mvprintw(row/2/2/2,cal/2-4,"ВНИМАНИЕ");
			attroff(A_BLINK);
			mvprintw(row/2/2/2+2,cal/2-14,"этот проект просто взорвет");
			attron(COLOR_PAIR(2));
			if(vvv){
				halfdelay(10);
				getch();
			}
			mvprintw(row/2/2/2+4,cal/2-11,"ВЗОРВЕТ ");
			if(vvv) getch();
			printw("ВАШУ ");
			if(vvv) getch();
			printw("ГОЛОВУ");
			if(vvv){
				getch();
				raw();
			}
		}

		attron(COLOR_PAIR(1));
		mvprintw(row/2/2/2+6,cal/2-9,"↑↓ матать строчки");
		mvprintw(row/2/2/2+8,cal/2-13,"←→ переход к документам");

		switch(getch()){
			case ERR: break;
			case 410:
				vvv=false;
				checksmallscreen();
			break;
			default: vv=false; break;
		}
	}
}

void filesmenu(){
	bool vv=true;
	int ifilesmenu=1;
	char buffer[50];
	while(vv){
		clear();
		attron(COLOR_PAIR(1));
		mvprintw(row/2-iinit,0,"┏");
		for(int i=0;i<cal-2;i++) printw("━");
		printw("┓");
		for(int i=0;i<iinit;i++){
			mvprintw(row/2-iinit+1+i,0,"┃");
			mvprintw(row/2-iinit+1+i,cal-1,"┃");
		}
		mvprintw(row/2,0,"┗");
		for(int i=0;i<cal-2;i++) printw("━");
		printw("┛");

		for(int i=1;i<iinit;i++){
			if(i==ifilesmenu) attron(COLOR_PAIR(2));
			else attron(COLOR_PAIR(3));
			sprintf(buffer,"%s",sdir[i].c_str());
			mvprintw(row/2-iinit+i,cal/2-14,"%s",buffer);
		}

		switch(getch()){
			case 'q': vv=false; break;
			case 'w':
			case 259:
				ifilesmenu--;
				if(ifilesmenu==0) ifilesmenu=iinit-1;
			break;
			case 's':
			case 258:
				ifilesmenu++;
				if(ifilesmenu==iinit) ifilesmenu=1;
			break;
			case 10:
				ifile=ifilesmenu;
				vv=false;
			break;
		}
	}
}

void initprogram(){
    checksmallscreen();
    if(iflag==1){
        matem=false,visual=false;
        printw("ошибка инициализации. думается что вы должны вести путь к папке с текстовыми документами\nпочему-то вы это не сделали\nпример: ./main /home/useranon/Documents");
        getch();
    }
    if(matem){
        fs::path path(spwd);

        if(!fs::directory_entry(path).is_directory()) {
            visual=false,matem=false;
            printw("ошибка инициализации файлов. Вы не правильно ввели путь");
            getch();
        } else {
            for(auto const& file: fs::directory_iterator(path)) {
                if(file.path().extension() == ".txt") {
                    ++iinit;
                    sdir[iinit] = file.path().filename();
                }
            }
            ++iinit; //The kludge ;>

            if(sdir[1].empty()){
                visual=false,matem=false;
                printw("ошибка инициализации файлов. нету файлов");
                getch();
            }

            if(visual){
                for(int i=1;i<iinit;i++){
                    attron(COLOR_PAIR(1));
                    mvprintw(row/2/2,cal/2-20,"инициализация текстовых файлов");
                    halfdelay(20);
                    mvprintw(row/2-iinit+i,cal/2-10,"%s",sdir[i].c_str());
                    getch();
                }
                raw();
            }
        }
    }
}

void draw(){
	clear();
	getmaxyx(stdscr,row,cal);
	attron(COLOR_PAIR(1));
	mvprintw(0,cal/2/2,"┏");
	for(int i=0;i<cal/2-2;i++) printw("━");
	printw("┓");
	mvprintw(1,cal/2/2,"┃");
	mvprintw(1,cal/2+cal/2/2-1,"┃");
	mvprintw(2,0,"┏");
	for(int i=0;i<cal/2/2-1;i++) printw("━");
	printw("┻");
	for(int i=0;i<cal/2-2;i++) printw("━");
	printw("┻");
	for(int i=0;i<cal/2/2;i++) printw("━");
	printw("┓");
	for(int i=3;i<row-3;i++){
		mvprintw(i,0,"┃");
		mvprintw(i,cal-1,"┃");
	}
	mvprintw(row-3,0,"┗");
	for(int i=0;i<cal-2;i++) printw("━");
	mvprintw(row-3,cal-1,"┛");
	mvprintw(row-3,cal/2-3,"┳");
	mvprintw(row-3,cal/2+3,"┳");
	mvprintw(row-3,cal/2/2/2-3,"┳");
	mvprintw(row-3,cal/2/2/2+3,"┳");
	mvprintw(row-3,cal/2+cal/2/2/2,"┳");
	mvprintw(row-3,cal/2+cal/2/2/2+cal/2/2,"┳");
	mvprintw(row-2,cal/2-3,"┃");
	mvprintw(row-2,cal/2+3,"┃");
	mvprintw(row-2,cal/2/2/2-3,"┃");
	mvprintw(row-2,cal/2/2/2+3,"┃");
	mvprintw(row-2,cal/2+cal/2/2/2,"┃");
	mvprintw(row-2,cal/2+cal/2/2/2+cal/2/2,"┃");
	mvprintw(row-1,cal/2-3,"┗");
	for(int i=0;i<5;i++) printw("━");
	mvprintw(row-1,cal/2+3,"┛");
	mvprintw(row-1,cal/2/2/2-3,"┗");
	for(int i=0;i<5;i++) printw("━");
	mvprintw(row-1,cal/2/2/2+3,"┛");
	mvprintw(row-1,cal/2+cal/2/2/2,"┗");
	for(int i=0;i<cal/2/2;i++) printw("━");
	mvprintw(row-1,cal/2+cal/2/2/2+cal/2/2,"┛");
}

void initwords(){
	if(visual) mvprintw(row/2+iinit+1,cal/2-8,"инициализация слов");
	iword=0,ititle=1;
	btitle=false;
	bool binit=true,bbtitle=false;
	char buffer[255];
	sprintf(buffer,"%s/%s",spwd.c_str(),sdir[ifile].c_str());
	fstream fword(buffer);
	while(binit){
		if(!fword.eof()){
			iword++;
			if(bbtitle) ititle++;
		}
		fword >> sword[iword];
		if(sword[iword]=="ЗАГОЛОВОК"){
			btitle=true;
			if(!bbtitle) bbtitle=true;
			else bbtitle=false;
		}
		if(fword.eof()){
			binit=false;
			fword.close();
		}
	}
}

void initscreen(){
	bool gg=true;
	while(gg){
		clear();
		attron(COLOR_PAIR(3));
		printw("закончил изменять размер экрана туда сюда?\n нажми любую кнопку для продолжения");
		switch(getch()){
			case 410: checksmallscreen(); break;
			default: gg=false; break;
		}
	}
}

void output(const string& outsay,bool effect){
	char csay[50];
	sprintf(csay,"%s",outsay.c_str());
	attron(COLOR_PAIR(3));
	if(effect){
		if(outsay=="МИГАНИЕ") attron(WA_BLINK);
		if(outsay=="ЖИРНЫЙ") attron(WA_BOLD);
		if(outsay=="ПОДЧЕРКНУТ") attron(WA_UNDERLINE);
	}
	if(outsay!="ЗАГОЛОВОК" && outsay!="МИГАНИЕ" && outsay!="ЖИРНЫЙ" && outsay!="ПОДЧЕРКНУТ") printw("%s ",csay);
	if(effect){
		if(outsay!="МИГАНИЕ") attroff(WA_BLINK);
		if(outsay!="ЖИРНЫЙ") attroff(WA_BOLD);
		if(outsay!="ПОДЧЕРКНУТ") attroff(WA_UNDERLINE);
	}
}

void initline(){
	clear();
	allline=2;
	int ibuffer;
	char buffer[150];
	for(int i=0;i<1000;i++) iline[i]=0;
	int tmprow=2;
	if(btitle) iline[1]=ititle;
	else iline[1]=1,
	mvprintw(1,0," ");
	if(btitle){
		for(int i=1;i<ititle;i++) output(sword[i],false);
		mvprintw(2,0," ");
	}
	for(int i=iline[1];i<iword;i++){
		getyx(stdscr,arow,acal);
		sprintf(buffer,"%s",sword[i].c_str());
		ibuffer=strlen(buffer);
		if(acal>cal-cal/2/2/2 || ibuffer>cal-acal){
			iline[allline]=i;
			allline++;
			if(arow==row-4) clear();
			else{
				tmprow++;
				mvprintw(tmprow,0," ");
			}
		}
		output(sword[i],false);
	}
}

int main(int argc,char* argv[]){

	//base
	setlocale(LC_ALL,"");
	initscr();
	curs_set(0);
	keypad(stdscr,true);
	noecho();
	raw();
	getmaxyx(stdscr,row,cal);
	start_color();
	init_color(COLOR_YELLOW,1000,600,0);
	init_pair(1,COLOR_YELLOW,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_BLACK);
	init_pair(3,COLOR_WHITE,COLOR_BLACK);
	iflag=argc;
	if(iflag!=1) spwd=argv[1];

	//base x2
	initprogram();
	if(matem){
		initwords();
		initline();
	}
	if(visual) vhelp();
	int iiline=1;

	//program
	while(matem){

		//draw
		draw();

		//logic
		if(!onematem){
			int tmprow=3,ibuffer;
			char buffer[50];
			if(btitle){
				attron(COLOR_PAIR(1));
				mvprintw(3,0,"┃");
				tmprow=4;
				
				for(int i=1;i<ititle;i++){
					output(sword[i],true);
				}
				attron(COLOR_PAIR(1));
				mvprintw(4,0,"┃");

			}
			else mvprintw(3,0,"┃");
			for(int i=iline[iiline];i<iword;i++){
				getyx(stdscr,arow,acal);
				sprintf(buffer,"%s",sword[i].c_str());
				ibuffer=strlen(buffer);
				if(acal>cal-cal/2/2/2 || ibuffer>cal-acal){
					tmprow++;
					attron(COLOR_PAIR(1));
					mvprintw(tmprow,0,"┃");
					if(arow==row-4) i=iword;
				}
				output(sword[i],true);
			}

			attron(COLOR_PAIR(2));
			mvprintw(1,cal/2-4,"нажми f2");

			if(ifile+1>=10) mvprintw(row-2,cal/2-1,"%d",ifile); 
			else{
			mvprintw(row-2,cal/2,"%d",ifile);
			if(ifile>=2) mvprintw(row-2,cal/2-1,"%d",ifile-1);
			if(ifile!=iinit-1) mvprintw(row-2,cal/2+1,"%d",ifile+1);
			}
			mvprintw(row-2,cal/2-2,"←");
			mvprintw(row-2,cal/2+2,"→");

			mvprintw(row-2,cal/2/2/2-2,"↑");
			mvprintw(row-2,cal/2/2/2,"%d",iiline);
			mvprintw(row-2,cal/2/2/2+2,"↓");

			sprintf(buffer,"%s",sdir[ifile].c_str());
			mvprintw(row-2,cal/2+cal/2/2/2+1,"%s",buffer);

			//dibag
			

			switch(getch()){
				case 265: vhelp(); break;
				case 267: 
					filesmenu();
					initwords();
					initline();
				break;
				case 410:
					initscreen();
					initline();
				break;
				case 'q': matem=false; break;
				case 'w':
				case 259:
					iiline--;
					if(iiline==0) iiline=1;
				break;
				case 's':
				case 258:
					iiline++;
					if(iiline==allline-1) iiline=allline-2;
				break;
				case 'd':
				case 261:
					ifile++,iiline=1;
					if(ifile==iinit) ifile=iinit-1;
					else{
						initwords();
						initline();
					}
				break;
				case 'a':
				case 260:
					ifile--,iiline=1;
					if(ifile==0) ifile=1;
					else{
						initwords();
						initline();
					}
				break;
				case 266:
					onematem=true;
				break;
			}
		}
		else{
			//init
			int One[50],Two[50],tmpnumber=0,answer=0;
			One[0]=onenumber,Two[0]=twonumber;
			if(onenumber&1) answer=twonumber;
			for(int i=0;i<onenumber;i++){
				tmpnumber++;
				One[i+1]=One[i]/2;
				if(One[i+1]==1) i=onenumber;
			}
			if(onenumber!=1) tmpnumber++;
			for(int i=0;i<tmpnumber;i++) Two[i+1]=Two[i]*2;
			for(int i=0;i<tmpnumber-1;i++) if(One[i+1]&1) answer=answer+Two[i+1];
			

			//draw one
			if(boolnumber) attron(WA_REVERSE);
			mvprintw(1,cal/2/2+1,"%d",onenumber);
			if(boolnumber) attroff(WA_REVERSE);
			printw("*");
			if(!boolnumber) attron(WA_REVERSE);
			printw("%d",twonumber);
			if(!boolnumber) attroff(WA_REVERSE);

			//draw two
			for(int i=0;i<tmpnumber;i++){
				if(One[i]&1) attroff(WA_DIM);
				else attron(WA_DIM);
				mvprintw(row/2-tmpnumber/2+i,cal/2,"%d*%d",One[i],Two[i]);
			}
			attroff(WA_DIM);

			mvprintw(row/2+tmpnumber,cal/2,"%d",answer);

			switch(getch()){
				case 'q': matem=false; break;
				case 's':
				case 258:
					if(boolnumber){
						onenumber--;
						if(onenumber==0) onenumber=1;
					}
					else{
						twonumber--;
						if(twonumber==0) twonumber=1;
					}
				break;
				case 'w':
				case 259:
					if(boolnumber) onenumber++;
					else twonumber++;
				break;
				case 'a':
				case 260:
					if(boolnumber) boolnumber=false;
					else boolnumber=true;
				break;
				case 'd':
				case 261:
					if(boolnumber) boolnumber=false;
					else boolnumber=true;
				break;
				case 266:
					onematem=false;
				break;
				case 410:
					initscreen();
				break;
			}
		}
	}

	endwin();
	return 0;
}



/*
		    ┏━━━━━━━━━━━━━┓
		    ┃  press f2   ┃
┏━━━━━━━━━━━┻━━━━━━━━━━━━━┻━━━━━━━━━━━┓
┃текст			   					  ┃
┃				   				      ┃
┃ 				 		    	      ┃
┃      			   				      ┃
┗━━━━┳━━━━━┳━━━━━┳━━━┳━┳━━━━━━━━━━━━┳━┛
	 ┃↑123↓┃     ┃123┃ ┃1_доки_рy.tx┃
	 ┗━━━━━┛	 ┗━━━┛ ┗━━━━━━━━━━━━┛
  ┣   ┫  ┃
 ╋ ━ ┻ ┳ ┏ ┓
    ┛┗

WA_ALTCHARSET Альтернативный набор символов
WA_BLINK Мигает
WA_BOLD Очень яркий или жирный
WA_DIM Половина яркости
WA_HORIZONTAL Горизонтальное выделение
WA_INVIS Невидимый
WA_LEFT Выделение слева
WA_LOW Низкое выделение
WA_PROTECT Защищено
WA_REVERSE Обратное видео
WA_RIGHT Правая подсветка
WA_STANDOUT Лучший режим подсветки терминала
WA_TOP Самое интересное
WA_UNDERLINE Подчеркивание
WA_VERTICAL Вертикальное выделение

A_ALTCHARSET Альтернативный набор символов
A_BLINK Мигает
A_BOLD Очень яркий или жирный
A_DIM Полуяркий
A_INVIS невидимый
A_PROTECT Защищено
A_REVERSE Обратное видео
A_STANDOUT Лучший режим подсветки терминала
A_UNDERLINE Подчеркивание

*/
