#include <regex.h>

/**
 * Estruturas de mapeamento do arquivo gpx
 */
struct trkpt_head{
    struct trkpt *next, *prev;
    double lon, lat, ele, tim;
};

struct trkseg{
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
 * Essa função precisa de um e somente um grupo na expressão regular!
 * retorna a struct regmatch_t com os limites da substring encontrada
 */
regmatch_t rxmatch(char *s,char *pat){
    regex_t rx;
    regmatch_t res[2];
    regcomp(&rx,pat,REG_EXTENDED);
    regexec(&rx,s,2,res,0);
    regfree(&rx);
    return res[1];
}

void gpxDecoder(char string[]){
    char res[1000];
    regmatch_t result = rxmatch(string, "<gpx>(.*?)</gpx>");
    int len = result.rm_eo - result.rm_so;
    memcpy(res, string + result.rm_so, len);
    res[len] = '\0';
    printf("%s\n", res);

    result = rxmatch(res, "<trk>(.*)</trk>");
    len = result.rm_eo - result.rm_so;
    memcpy(res, res + result.rm_so, len);
    res[len] = '\0';
    printf("%s\n", res);

    char name[10];
    result = rxmatch(res, "<name>(.*)</name>");
    len = result.rm_eo - result.rm_so;
    memcpy(name, res + result.rm_so, len);
    name[len] = '\0';
    printf("%s\n", name);

    char trkseg[1000];
    result = rxmatch(res, "<trkseg>(.*)</trkseg>");
    len = result.rm_eo - result.rm_so;
    memcpy(trkseg, res + result.rm_so, len);
    trkseg[len] = '\0';
    printf("%s\n", trkseg);

    char trkpt[1000];
    result = rxmatch(res, "<trkpt (.*)</trkpt><trkpt");
    len = result.rm_eo - result.rm_so;
    memcpy(trkpt, res + result.rm_so, len);
    trkpt[len] = '\0';
    printf("%s\n", trkpt);

    char lon[30];
    result = rxmatch(res, "lon=.(-?+?[0-9]+.\?[0-9]*).");
    len = result.rm_eo - result.rm_so;
    memcpy(lon, res + result.rm_so, len);
    lon[len] = '\0';
    printf("%s\n", lon);

    char lat[30];
    result = rxmatch(trkpt, "lat=.(-?+?[0-9]+.\?[0-9]*).");
    len = result.rm_eo - result.rm_so;
    memcpy(lat, trkpt + result.rm_so, len);
    lat[len] = '\0';
    printf("%s\n", lat);

    char ele[30];
    result = rxmatch(trkpt, "<ele>(.*)</ele>");
    len = result.rm_eo - result.rm_so;
    memcpy(ele, trkpt + result.rm_so, len);
    ele[len] = '\0';
    printf("%s\n", ele);

    char tim[30];
    result = rxmatch(trkpt, "<time>(.*)</time>");
    len = result.rm_eo - result.rm_so;
    memcpy(tim, trkpt + result.rm_so, len);
    tim[len] = '\0';
    printf("%s\n", tim);

    struct trkpt_head trkpth = {0, 0, strtodbl(lon), strtodbl(lat), strtodbl(ele), strtodbl(tim)};

    struct trkseg trksg = {&trkpth};

    struct trk_head trkh = {0, 0, &trksg, name};

    struct gpx gpxx = {&trkh};

}



int main(){
    char arquivo[] = "<gpx><trk><name>SUP</name><trkseg><trkpt lon='-49.38176058232784' lat='-25.47733305953443'><ele>894.7999877929688</ele><time>2014-01-19T12:24:32.000Z</time></trkpt><trkpt lon='-49.381785560399294' lat='-25.477311853319407'><ele>894.7999877929688</ele><time>2014-01-19T12:24:37.000Z</time></trkpt></trkseg></trk></gpx>";

    gpxDecoder(arquivo);

 return 0;

}
