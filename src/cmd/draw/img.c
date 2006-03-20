#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>

void
usage(void)
{
	fprint(2, "usage: img [-W winsize] [file]\n");
	exits("usage");
}

Image *image;

void
eresized(int new)
{
	if(new && getwindow(display, Refnone) < 0)
		sysfatal("can't reattach to window: %r");

	draw(screen, screen->r, display->white, nil, ZP);
	drawop(screen, screen->r, image, nil, image->r.min, S);
}

void
main(int argc, char **argv)
{
	int fd;
	Event e;

	ARGBEGIN{
	case 'W':
		winsize = EARGF(usage());
		break;
	default:
		usage();
	}ARGEND

	if(argc > 1)
		usage();

	if(argc == 1){
		if((fd = open(argv[0], OREAD)) < 0)
			sysfatal("open %s: %r");
	}else
		fd = 0;

	if(initdraw(0, nil, nil) < 0)
		sysfatal("initdraw: %r");

	if((image=readimage(display, fd, 0)) == nil)
		sysfatal("readimage: %r");

	drawresizewindow(Rect(0,0,Dx(image->r),Dy(image->r)));

	einit(Emouse|Ekeyboard);
	eresized(0);
	for(;;){
		switch(event(&e)){
		case Ekeyboard:
			if(e.kbdc == 'q' || e.kbdc == 0x7F)
				exits(nil);
			eresized(0);
			break;
		case Emouse:
			break;
		}
	}
}
