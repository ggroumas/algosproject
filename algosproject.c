#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Typos gia mia genikeymenh lista. Mporei na dexetai otidhpote antikeimena
 * arkei na kseroyme to megethos toys.
 */
typedef struct my_list * list;
struct my_list {
    void * data;
    list next;
};

/* Dhmioyrgei mia nea lista kai thn epistrefei. Arxikopoiei toys deiktes se NULL */
list list_new() {
    list result;
    result = malloc(sizeof (struct my_list));
    result->data = NULL;
    result->next = NULL;
    return result;
}

/* Apeleytheronei mia lista kai ta antikeimena poy periexei. */
void list_free(list head) {
    list node = head, tmp;
    if (head == NULL)
        return;
    while (node != NULL) {
        free(node->data);
        tmp = node;
        node = node->next;
        free(tmp);
    }
}

/* Prosthetei ena neo antikeimeno sto telos mias listas.
 * An to head einai NULL, tote desmeyetai neos kombos, alliws briskoyme
 * ton kombo poy to next toy einai NULL, ekei desmeyoyme ena neo kombo kai
 * kanoyme thn anathesh (me memcpy) twn data
 */
list list_add(list head, void * data, int data_size) {
    list node;
    if (head == NULL) {
        head = node = list_new();
    } else {
        node = head;
        while (node->next != NULL)
            node = node->next;
        node->next = list_new();
        node = node->next;
    }
    node->data = malloc(data_size);
    memcpy(node->data, data, data_size);
    return head;
}

/* Anaparista mia eggrafh se pinaka katakermatismoy. Gia na mporoyme na ftiaxnoyme
 * pinakes diaforwn eidwn, to periexomeno mias theshs einai mia genikeymenh lista.
 */
struct hash_entry {
    char * key;
    list elements;
};

/* Anaparista ton pinaka katakermatismoy. Einai sthn oysia enas pinakas apo
 * hash_entry.
 */
typedef struct my_hash * hash;
struct my_hash {
	int conflicts;
    int num_items;
    struct hash_entry * items;
};

/* Ypologizei ena aplo hash function dedomenoy toy kleidioy str kai toy megistoy
 * max.
 */
unsigned int hash1(const char *str, int max)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % max;
}

/* Dhmioyrgei ena neo pinaka katakermatismoy me ayto ton arithmo apo items. */
hash hash_new(int num_items) {
    hash result;
    result = malloc(sizeof(struct my_hash));
    result->items = calloc(1, num_items * sizeof(struct hash_entry));
    result->num_items = num_items;
    result->conflicts = 0;
    return result;
}

/* Topothetei mia eggrafh ston pinaka katakermatismoy. H politikh topothetisis
 * einai ws ekshs:
 * H hash1 synarthsh epistrefei mia thesh.
 * An sth thesh ayth yparxei kleidi diaforetiko apo to key tote:
 *   Gia oso to kleidi ths epomenhs theshs den einai keno kai einai diaforetiko
 *   apo to dedomeno, proxwrame mia thesh. An kseperasoyme to orio thesewn pame
 *   pali sto 0. Etsi tha broyme h mia kenh thesh, h mia thesh poy na exei to idio
 *   kleidi me to dedomeno.
 * Sthn thesh ayth kanoyme prosthiki ths eggrafhs sth lista elements poy ths
 * antistoixei.
 */
void hash_put(hash h, char * key, void * data, int data_size) {
    int index;

    index = hash1(key, h->num_items);
    if (h->items[index].key == NULL) {
        h->items[index].key = strdup(key);
    } else {
        if (strcmp(h->items[index].key, key) != 0) {
			h->conflicts++;
            while (h->items[index].key != NULL && strcmp(h->items[index].key, key) != 0)
                index = (index + 1) % h->num_items;
		}
    }
    h->items[index].elements = list_add(h->items[index].elements, data, data_size);
}

/* Epistrefei ta stoixeia poy antistoixoyn sto kleidi key.
 * H politikh epistrofhs einai os ekshs:
 * H hash1 synarthsh epistrefei mia thesh.
 * An sth thesh ayth yparxei kleidi diaforetiko apo to key tote:
 *    Efoson to kleidi ths epomenhs theshs den einai idio me to key, tote proxorame
 *    mia thesh deksia, an kseperasoyme to orio pame pali sto 0.
 * Epistrefoyme to elements sth thesh ayth.
 */
list hash_get(hash h, char * key) {
    int index;
    int begin;
    begin = index = hash1(key, h->num_items);
    while (h->items[index].key == NULL || strcmp(h->items[index].key, key) != 0) {
        index = (index + 1) % h->num_items;
        if (index == begin) return NULL;
    }
    return h->items[index].elements;
}

/* Apodesmeyei ola ta desmeymena tmhmata toy pinaka katakermatismoy mas */
void hash_free(hash h) {
    int i;
    for (i=0; i<h->num_items; i++) {
        free(h->items[i].key);
        list_free(h->items[i].elements);
    }
    free(h->items);
    free(h);
}

void error(char * mhnyma) {
    printf("%s", mhnyma);
    exit(1);
}

struct proion_se_agora {
    char onoma[16];
    int arithmos;
};

struct agora {
    int hmera;
    list proionta;
};

struct omada {
    double syntelestis;
    list proionta;
};

struct timh {
    int kostos;
    int hmeres[8];
};

struct doro {
    char kodikos[16];
    int pontoi;
};

/* Edw tha fortothoyn ta dedomena apo ta arxeia. */
hash h_agores = NULL;
hash h_omades = NULL;
hash h_times = NULL;
list l_kartes_omadas = NULL;
list l_dora = NULL;
/* An einai fortomena ta arxeia ginetai 1, kai den mporei na ksanafortothoyn. */
int arxeia_fortomena = 0;

void clear(char *str) {
    while (*str) {
        if (*str == '\r' || *str == '\n')
            *str = ' ';
        str++;
    }
    // Bazoyme kai ena keno sto telos gia sigoyria oti katharise...
    strcat(str, " ");
}

void fortose_arxeia()
{
    FILE *f;
    char buffer[1024];
    char s1[256], s2[256], s3[256];
    char kodikos[16];
    char *c;
    int i, j, l, n;

    if (arxeia_fortomena) {
        printf("Ta arxeia einai hdh fortomena!\n");
        return;
    }
    // Arxeio 1
    printf("Fortonoyme ta dedomena apo to arxeio1.txt\n");
    f = fopen("arxeio1.txt", "r");
    if ( f == NULL ) error("To arxeio1.txt problima! Den anoigei!\n");
    h_agores = hash_new(1000113);
    while ( fgets(buffer, sizeof(buffer), f) != NULL ) {
        struct agora a;
        clear(buffer);
        sscanf(buffer, " %d %s %[^ ]", &i, s1, s2);
        a.hmera = i;
        strcpy(kodikos, s1);
        a.proionta = NULL;
        c = s2;
        while(*c) {
            struct proion_se_agora p;

            sscanf(c, "%[^,;],%[^,;];", s1, s3);
            c = c+strlen(s1)+strlen(s3)+2;
            p.arithmos = atoi(s3);
            strcpy(p.onoma, s1);
            a.proionta = list_add(a.proionta, &p, sizeof(p));
        }
        hash_put(h_agores, kodikos, &a, sizeof(a));
    }
    fclose(f);
    printf("Arxeio1 sth mnhmh.\n");
    // Arxeio2
    printf("Fortonoyme ta dedomena apo to arxeio2\n");
    f = fopen("arxeio2.txt", "r");
    if ( f == NULL ) error("To arxeio2.txt problima! Den anoigei!\n");
    h_omades = hash_new(1024);
    while( fgets(buffer, sizeof(buffer), f) !=  NULL ) {
        struct omada o;
        clear(buffer);
        sscanf(buffer, "%[^:]:%[^:]:%[^ ]", s1, s2, s3);
        strcpy(kodikos, s1);
        o.syntelestis = atof(s2);
        o.proionta = NULL;
        c = s3;
        while(*c) {
            i = 0;
            while (*c && *c != ',') {
                s1[i++] = *c;
                c++;
            }
            if (*c) c++;
            s1[i] = 0;
            o.proionta = list_add(o.proionta, s1, strlen(s1) + 1);
        }
        hash_put(h_omades, kodikos, &o, sizeof(o));
    }
    fclose(f);
    printf("Arxeio2 sth mnhmh\n");
    // Arxeio3
    printf("Fortonoyme ta dedomena apo to arxeio3\n");
    f = fopen("arxeio3.txt", "r");
    if ( f == NULL ) error("To arxeio3.txt problima! Den anoigei!\n");
    h_times = hash_new(2347);
    while ( fgets(buffer, sizeof(buffer), f) != NULL ) {
        struct timh t;
        clear(buffer);
        sscanf(buffer, "%s%s%[^{]", s1, s2, s3);
        strcpy(kodikos, s1);
        memset(t.hmeres, 0, sizeof(t.hmeres));
        t.kostos = atoi(s2);
        c = s3;
        while (*c) {
            int mera;
            while(*c && !isdigit(*c))
                c++;
            if (!*c)
                break;
            sscanf(c, " %d", &mera);
            while(*c && isdigit(*c))
                c++;
            t.hmeres[mera] = 1;
        }
        hash_put(h_times, kodikos, &t, sizeof(t));
    }
    fclose(f);
    printf("Arxeio3 sth mnhmh\n");
    // Arxeio4
    printf("Fortonoyme ta dedomena apo to arxeio4\n");
    f = fopen("arxeio4.txt", "r");
    if ( f == NULL ) error("To arxeio4.txt problima! Den anoigei!\n");
    l_kartes_omadas = NULL;
    while (fgets(buffer, sizeof(buffer), f) !=  NULL) {
        struct timh t;
        clear(buffer);
        fscanf(f, "%s", kodikos);
        l_kartes_omadas = list_add(l_kartes_omadas, kodikos, strlen(kodikos)+1);
    }
    fclose(f);
    printf("Arxeio4 sth mnhmh\n");
    // Arxeio5
    printf("Fortonoyme ta dedomena apo to arxeio5\n");
    f = fopen("arxeio5.txt", "r");
    if ( f == NULL ) error("To arxeio5.txt problima! Den anoigei!\n");
    l_dora = NULL;
    while (fgets(buffer, sizeof(buffer), f) !=  NULL) {
        struct doro d;
        clear(buffer);
        sscanf(buffer,"%s%s", s1, s2);
        strcpy(d.kodikos,s1);
        d.pontoi = atoi(s2);
        l_dora = list_add(l_dora, &d, sizeof(d));
    }
    fclose(f);
    printf("Arxeio5 sth mnhmh\n");
    printf("Ola ta arxeia einai tora fortomena.\n");
    arxeia_fortomena = 1;
}

double get_syntelesth(char * proion) {
    int i;
    for (i=0; i<h_omades->num_items; i++) {
        if (h_omades->items[i].key != NULL) {
            list l = h_omades->items[i].elements;
            while (l != NULL) {
                struct omada * o = l->data;
                list l2 = o->proionta;
                while (l2 != NULL) {
                    if (strcmp(proion, l2->data) == 0) {
                        return o->syntelestis;
					}
                    l2 = l2->next;
                }
                l = l->next;
            }
        }
    }
    return 0.5;
}

double get_timh(int hmera, char * proion) {
    list l = hash_get(h_times, proion);
    int kostos = 10;
    if (l != NULL) {
        while (l != NULL) {
            struct timh * t = l->data;
            if (t->hmeres[hmera] == 1)
                kostos = t->kostos;
            l = l->next;
        }
    }
    return kostos;
}

char * kartes[10];
double aksies[10];

void enhmerosi_top10(char * karta, double aksia) {
    int i;
    for (i=0; i<10; i++) {
        if (aksia > aksies[i]) {
            int j;
            for (j=8; j>=i; j--) {
                aksies[j+1] = aksies[j];
                kartes[j+1] = kartes[j];
            }
            aksies[i] = aksia;
            kartes[i] = karta;
            return;
        }
    }
}

double aksia_agorwn(list l) {
    double aksia_agoras = 0;

    while (l != NULL) {
        struct agora * a = l->data;
        list l2 = a->proionta;

        while (l2 != NULL) {
            struct proion_se_agora * p = l2->data;
            aksia_agoras += p->arithmos *
                get_syntelesth(p->onoma) * get_timh(a->hmera, p->onoma);
            l2 = l2->next;
        }
        l = l->next;
    }
    return aksia_agoras;
}

void kalyteres_10() {
    int i;
    // Katharizoyme tis kartes, an den einai kathares
    memset(kartes, 0, sizeof(kartes));
    memset(aksies, 0, sizeof(aksies));
    for (i=0; i<h_agores->num_items; i++) {
        if(h_agores->items[i].key != NULL) {
            double sum = aksia_agorwn(h_agores->items[i].elements);
            enhmerosi_top10(h_agores->items[i].key, sum);
        }
    }
    printf("Thesh\tKarta\tAgores\n");
    for (i=0; i<10; i++) {
        printf("%d) %s %f\n", i+1, kartes[i], aksies[i]);
    }
}

void dora_omadwn() {
    int i;
    double sum = 0;
    list l = l_kartes_omadas;

    while (l != NULL) {
        char * kodikos = l->data;
        list agores = hash_get(h_agores, kodikos);
        sum = sum + aksia_agorwn(agores);
        l = l->next;
    }
    printf("Oi kartes sygkentrwsan pontoys %f\n", sum);
    l = l_dora;
    while (l != NULL) {
        struct doro * d = l->data;
        if (sum >= d->pontoi)
            printf("Diakoynte to dwro: %s (%d)\n", d->kodikos, d->pontoi);
        else
			printf("Xanoyn to dwro: %s (%d)\n", d->kodikos, d->pontoi);        
        l = l->next;
    }
}

void sygkroyseis() {
	printf("Sygkroyseis ston pinaka agorwn: %d\n", h_agores->conflicts);
	printf("Sygkroyseis ston pinaka omadwn syntelestwn proiontwn: %d\n", h_omades->conflicts);
	printf("Sygkroyseis ston pinaka timwn proiontwn: %d\n", h_times->conflicts);
}

int main(int argc, char *argv[])
{
    int opt = 0;
    for(;;) {
        printf("1. Fortoste arxeia me dedomena (arxeio1.txt, ...)\n");
        printf("2. Breite tis 10 kartes me toys perissoteroys pontoys\n");
        printf("3. Breite ta dwra twn omadwn\n");
        printf("4. Breite poses sygkroyseis ginontai ston pinaka katakermatismoy\n");
        printf("5. Eksodos\n");
        printf("??? ");
        opt = getchar();
        while (getchar() != '\n') /* Tipota */ ;
        switch(opt) {
            case '1':
                fortose_arxeia();
                break;
            case '2':
                if (!arxeia_fortomena) {
                    printf("Ta arxeia den einai fortomena!\n");
                    continue;
                }
                kalyteres_10();
                break;
            case '3':
                if (!arxeia_fortomena) {
                    printf("Ta arxeia den einai fortomena!\n");
                    continue;
                }
                dora_omadwn();
                break;
            case '4':
                if (!arxeia_fortomena) {
                    printf("Ta arxeia den einai fortomena!\n");
                    continue;
                }
                sygkroyseis();
                break;
            case '5':
                printf("Kleisimo programmatos.\n");
                list_free(l_kartes_omadas);
                list_free(l_dora);
                hash_free(h_times);
                hash_free(h_omades);
                hash_free(h_agores);
                exit(0);
            default:
                printf("Patiste pliktra 1,2,3,4,5 mono parakalw!\n");
        }
    }
}
