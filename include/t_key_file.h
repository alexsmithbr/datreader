#include <iostream>
#include <vector>
#include <iomanip> // setw

using namespace std;

class t_key_file_header {
    public:
        unsigned int   root;       // number of root node
        unsigned int   numkent;    // number of key entries
        unsigned int   numnode;    // number of nodes for this index
        unsigned int   lastnod;    // node number of last node
        unsigned int   keyeof;     // record # of end of file
        unsigned int   keybof;     // record # of beginning of file
        unsigned int   unused;     // first unused node of file
        unsigned char  keytyp;     // type of key
        unsigned char  keynode;    // # of keys per node
        unsigned char  numcmps;    // # of components of key
        unsigned short keylen;     // total length of key entry
        unsigned short numlvls;    // number of levels
        char           cvoid[477]; // reserved space
};

class t_key_file_pointer_key {
    public:
        unsigned int relrec;   // record number/node number
        char key[];             // key value (key size - size of unsigned int)
};

class t_key_file_node {
    public:
        unsigned char keycnt;    // number of keys in this nodes
        unsigned int  flink;     // forward node pointer
        unsigned int  blink;     // backwards node pointer
        unsigned int  ulink;     // upwards node pointer

        vector < t_key_file_pointer_key * > keys;
};

class t_key_file {
    private:
        FILE *f;
        char *key_filename;

    public:
        t_key_file_header *header;
        t_key_file_node *node;
        
        t_key_file (char *filename) {
            this->key_filename = filename;

            this->f = fopen(this->key_filename,"r");
            if ( this->f == NULL ) {
                cout << "ERROR: (t_key_file): Couldn't open file." << endl;
            } else {
                this->header = new t_key_file_header();
                this->read_header();

                this->read_keys();
            }
        }

        void read_header() {
            fseek(this->f, 0, SEEK_SET);

            fread(&this->header->root,    sizeof(unsigned int),     1, this->f);
            fread(&this->header->numkent, sizeof(unsigned int),     1, this->f);
            fread(&this->header->numnode, sizeof(unsigned int),     1, this->f);
            fread(&this->header->lastnod, sizeof(unsigned int),     1, this->f);
            fread(&this->header->keyeof,  sizeof(unsigned int),     1, this->f);
            fread(&this->header->keybof,  sizeof(unsigned int),     1, this->f);
            fread(&this->header->unused,  sizeof(unsigned int),     1, this->f);
            fread(&this->header->keytyp,  sizeof(unsigned char),    1, this->f);
            fread(&this->header->keynode, sizeof(unsigned char),    1, this->f);
            fread(&this->header->numcmps, sizeof(unsigned char),    1, this->f);
            fread(&this->header->keylen,  sizeof(unsigned short),   1, this->f);
            fread(&this->header->numlvls, sizeof(unsigned short),   1, this->f);
            fread(&this->header->cvoid,   sizeof(unsigned char),  477, this->f);
        }

        void read_keys() {
            int i;

            // calculate the root node offset and position the file
            // pointer at that position
            fseek(this->f, 512 * this->header->root, SEEK_SET);
            
            this->node = new t_key_file_node();

            fread(&node->keycnt, sizeof(unsigned char), 1, this->f);
            fread(&node->flink,  sizeof(unsigned int),  1, this->f);
            fread(&node->blink,  sizeof(unsigned int),  1, this->f);
            fread(&node->ulink,  sizeof(unsigned int),  1, this->f);

            t_key_file_pointer_key *pointer_key;

            for ( i=0; i<node->keycnt; i++ ) {
                pointer_key = new t_key_file_pointer_key();

                fread(&pointer_key->relrec, sizeof(unsigned int), 1,                        this->f);
                fread(&pointer_key->key,    sizeof(char),         this->header->keylen - 4, this->f);

                cout << "key " << pointer_key->relrec << ": " << pointer_key->key << endl;

                node->keys.push_back(pointer_key);
            }
        }
};


