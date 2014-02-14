/**
 * Estruturas de mapeamento do arquivo gpx
 */
struct trkpt_head {
	struct trkpt *next, *prev;
	double lon, lat, ele, tim;
};

struct trkseg {
	struct trkpt_head *trkpts;
};

struct trk_head {
	struct trk_head *next, *prev;
	struct tkrseg *tkrsg;
	char *name;
};

struct gpx {
	struct trk_head *trks;
};

/**
 * Variáveis globais
 */
int flagGpxOpen = 0;
int flagTrkOpen = 0;
int flagNameOpen = 0;
int flagTrksegOpen = 0;
int flagTrkptOpen = 0;
int flagLonOpen = 0;
int flagLatOpen = 0;
int flagEleOpen = 0;
int flagTimeOpen = 0;
char buffer[11];
int pointer = 0;

void flagSelector(char* input) {
	switch (input) {
	case "<gpx":
		flagGpxOpen = 1;
		break;
	case "<trk":
			flagTrkOpen = 1;
			break;
	}
}

void analisador(char input) {
	if (pointer == 9) {
		pointer = 0;
	}
	if (input == '<') {
		pointer = 0;
	}
	buffer[pointer] = input;
	buffer[pointer + 1] = '\0';
}

void extract(char input) {

}

/**
 * Main function - teste!!
 */
int main() {
	char arquivo[] =
			"<gpx><trk><name>SUP</name><trkseg><trkpt lon='-49.38176058232784' lat='-25.47733305953443'><ele>894.7999877929688</ele><time>2014-01-19T12:24:32.000Z</time></trkpt><trkpt lon='-49.381785560399294' lat='-25.477311853319407'><ele>894.7999877929688</ele><time>2014-01-19T12:24:37.000Z</time></trkpt></trkseg></trk></gpx>";

	extract(arquivo);

	return 0;

}
