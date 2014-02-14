#include <regex.h>

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

regmatch_t rxmatch(char *s,char *pat){
    regex_t rx;
    regmatch_t res;
    regcomp(&rx,pat,REG_EXTENDED);
    regexec(&rx,s,1,&res,0);
    regfree(&rx);
    return res;
}

void gpxDecoder(char string[]){
    char res[340];
    regmatch_t result = rxmatch(string, "<trkpt.*?>");
    int len = result.rm_eo - result.rm_so;
    memcpy(res, string + result.rm_so, len);
    printf("'%s'\n", res);
}



int main(){
    char arquivo[] = "<gpx><trk><name>SUP</name><trkseg><trkpt lon='-49.38176058232784' lat='-25.47733305953443'><ele>894.7999877929688</ele><time>2014-01-19T12:24:32.000Z</time></trkpt><trkpt lon='-49.381785560399294' lat='-25.477311853319407'><ele>894.7999877929688</ele><time>2014-01-19T12:24:37.000Z</time></trkpt></trkseg></trk></gpx>";

    gpxDecoder(arquivo);

 return 0;

}
