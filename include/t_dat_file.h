#include <iostream>
#include <vector>
#include <iomanip> // setw

using namespace std;

class t_dat_file_header {
    public:
        // header structure
        unsigned short filesig;     // file signature
        unsigned short sfatr;       // file attribute and status
        unsigned char  numkeys;     // number of keys in file
        unsigned int   numrecs;     // number of records in file
        unsigned int   numdels;     // number of deleted records
        unsigned short numflds;     // number of fields
        unsigned short numpics;     // number of pictures
        unsigned short numarrs;     // number of arrays
        unsigned short reclen;      // record length (including record header)
        unsigned int   offset;      // start of data area
        unsigned int   logeof;      // logical end of file
        unsigned int   logbof;      // logical start of file
        unsigned int   freerec;     // first usable deleted record
        unsigned char  recname[12]; // record name w/o prefix
        unsigned char  memnam[12];  // memo name w/o prefix
        unsigned char  filpre[3];   // file name prefix
        unsigned char  recpre[3];   // record name prefix
        unsigned short memolen;     // size of memo
        unsigned short memowid;     // column width of memo
        unsigned int   reserved;    // reserved
        unsigned int   chgtime;     // time of last change
        unsigned int   chgdate;     // date of last change
        unsigned short reserved2;   // reserved

        // class methods
        unsigned int get_total_keys() {
            return this->numkeys;
        }

        unsigned int get_total_records() {
            return this->numrecs;
        }

        unsigned int get_total_fields() {
            return this->numflds;
        }
        
        unsigned int get_total_pictures() {
            return this->numpics;
        }

        unsigned int get_total_arrays() {
            return this->numarrs;
        }

        unsigned int get_last_change_date() {
            return this->chgdate;
        }

        unsigned int get_last_change_time() {
            return this->chgtime;
        }
        
        unsigned int get_data_start_offset() {
            return this->offset;
        }
        
        unsigned short get_record_length() {
            return this->reclen;
        }
};

class t_dat_file_field {
    public:
        // field descriptor
        unsigned char  fldtype;     // type of field
        unsigned char  fldname[16]; // name of field
        unsigned short foffset;     // offset into record
        unsigned short length;      // length of field
        unsigned char  decsig;      // significance for decimals
        unsigned char  decdec;      // number of decimal places
        unsigned short arrnum;      // array number
        unsigned short picnum;      // picture number

        char *get_field_type_desc() {
            switch ( this->fldtype ) {
                case 1:
                    return "LONG";
                    break;
                case 2:
                    return "REAL";
                    break;
                case 3:
                    return "STRING";
                    break;
                case 4:
                    return "STRING WITH PICTURE TOKEN";
                    break;
                case 5:
                    return "BYTE";
                    break;
                case 6:
                    return "SHORT";
                    break;
                case 7:
                    return "GROUP";
                    break;
                case 8:
                    return "DECIMAL";
                    break;
                default:
                    return "invalid";
            }
        }

        unsigned char get_field_type() {
            return this->fldtype;
        }
        
        unsigned char *get_field_name() {
            return this->fldname;
        }
        
        unsigned short get_field_length() {
            return this->length;
        }
};

class t_dat_file_keypart {
    public:
        unsigned char  fldtype; // type of field
        unsigned short fldnum;  // field number
        unsigned short elmoff;  // record offset of this element
        unsigned char  elmlen;  // length of element
};

class t_dat_file_keysect {
    public:
        unsigned char numcomps;    // number of components for key
        unsigned char keyname[16]; // name of this key
        unsigned char comptype;    // type of composite
        unsigned char complen;     // length of composite
        vector < t_dat_file_keypart * > keyparts;
};

class t_dat_file_field_array_part {
    public:
        unsigned short maxdim; // number of dims for array part
        unsigned short lendim; // length of field
};

class t_dat_file_field_array {
    public:
        unsigned short numdim; // dims for current field
        unsigned short totdim; // total number of dims for field
        unsigned short elmsiz; // total size of current field

        vector < t_dat_file_field_array_part * > array_parts;
};

class t_dat_file {
    private:
        FILE *f;
        char *dat_filename;

    public:
        // header
        t_dat_file_header *header;
        // fields vector
        vector < t_dat_file_field * > fields;
        // keys vector
        vector < t_dat_file_keysect * > keys;

        // t_dat_file class constructor
        t_dat_file(char *filename) {
            this->dat_filename = filename;

            this->f = fopen(this->dat_filename,"r");
            if ( this->f == NULL ) {
                cout << "ERROR: (t_dat_file): Couldn't open file." << endl;
            } else {
                header = new t_dat_file_header();
                this->read_header();
                this->read_fields();
                this->read_keys();
            }
        }

        // method to read the .dat file header and
        // set the class attributes
        void read_header() {
            fseek(this->f, 0, SEEK_SET);

            fread(&this->header->filesig,   sizeof(unsigned short), 1,this->f);
            fread(&this->header->sfatr,     sizeof(unsigned short), 1,this->f);
            fread(&this->header->numkeys,   sizeof(unsigned char),  1,this->f);
            fread(&this->header->numrecs,   sizeof(unsigned int),   1,this->f);
            fread(&this->header->numdels,   sizeof(unsigned int),   1,this->f);
            fread(&this->header->numflds,   sizeof(unsigned short), 1,this->f);
            fread(&this->header->numpics,   sizeof(unsigned short), 1,this->f);
            fread(&this->header->numarrs,   sizeof(unsigned short), 1,this->f);
            fread(&this->header->reclen,    sizeof(unsigned short), 1,this->f);
            fread(&this->header->offset,    sizeof(unsigned int),   1,this->f);
            fread(&this->header->logeof,    sizeof(unsigned int),   1,this->f);
            fread(&this->header->logbof,    sizeof(unsigned int),   1,this->f);
            fread(&this->header->freerec,   sizeof(unsigned int),   1,this->f);
            fread(&this->header->recname,   sizeof(unsigned char), 12,this->f);
            fread(&this->header->memnam,    sizeof(unsigned char), 12,this->f);
            fread(&this->header->filpre,    sizeof(unsigned char),  3,this->f);
            fread(&this->header->recpre,    sizeof(unsigned char),  3,this->f);
            fread(&this->header->memolen,   sizeof(unsigned short), 1,this->f);
            fread(&this->header->memowid,   sizeof(unsigned short), 1,this->f);
            fread(&this->header->reserved,  sizeof(unsigned int),   1,this->f);
            fread(&this->header->chgtime,   sizeof(unsigned int),   1,this->f);
            fread(&this->header->chgdate,   sizeof(unsigned int),   1,this->f);
            fread(&this->header->reserved2, sizeof(unsigned short), 1,this->f);
        }

        // read every field descriptor of the .dat file and
        // put them into a vector class attribute called fields.
        void read_fields() {
            unsigned int i;

            t_dat_file_field *fld;

            for (i=0; i<this->header->get_total_fields(); i++ )
            {
                fld = new t_dat_file_field();

                fread(&fld->fldtype, sizeof(unsigned char),  1,this->f);
                fread(&fld->fldname, sizeof(unsigned char), 16,this->f);
                fread(&fld->foffset, sizeof(unsigned short), 1,this->f);
                fread(&fld->length,  sizeof(unsigned short), 1,this->f);
                fread(&fld->decsig,  sizeof(unsigned char),  1,this->f);
                fread(&fld->decdec,  sizeof(unsigned char),  1,this->f);
                fread(&fld->arrnum,  sizeof(unsigned short), 1,this->f);
                fread(&fld->picnum,  sizeof(unsigned short), 1,this->f);

                this->fields.push_back(fld);
            }
        }

        // read all information about keys present in .dat file and put
        // it in a vector class attribute called keys.
        void read_keys() {
            unsigned int i, j;

            t_dat_file_keysect *keysect;
            t_dat_file_keypart *keypart;

            for (i=0; i<this->header->get_total_keys(); i++ ) {
                keysect = new t_dat_file_keysect();

                fread(&keysect->numcomps, sizeof(unsigned char),  1,this->f);
                fread(&keysect->keyname,  sizeof(unsigned char), 16,this->f);
                fread(&keysect->comptype, sizeof(unsigned char),  1,this->f);
                fread(&keysect->complen,  sizeof(unsigned char),  1,this->f);

                for (j=0; j<keysect->numcomps; j++ ) {
                    keypart = new t_dat_file_keypart();

                    fread(&keypart->fldtype, sizeof(unsigned char),  1,this->f);
                    fread(&keypart->fldnum,  sizeof(unsigned short), 1,this->f);
                    fread(&keypart->elmoff,  sizeof(unsigned short), 1,this->f);
                    fread(&keypart->elmlen,  sizeof(unsigned char),  1,this->f);

                    keysect->keyparts.push_back(keypart);
                }
                    
                this->keys.push_back(keysect);
            }
        }
        
        // retrieve the rec_no'th record of .dat file
        // if parameter actives_only = 1 then show only
        // non-deleted records.
        void get_records_as_table(int actives_only=0) {
            // record header definition
            unsigned char rec_status; // record header type and status
            unsigned int  rec_ptr;    // point to next deleted record or memo if active

            unsigned int i, j;

            // record counter
            unsigned int rec_no;
            
            // variables
            char *fld_string;
            unsigned short fld_short;

            cout << "RECORD_ID       ;";
            for ( i=0; i<this->header->get_total_fields(); i++ ) {
                cout << this->fields.at(i)->get_field_name() << ";";
            }
            cout << endl;

            for ( rec_no=0; rec_no < this->header->get_total_records(); rec_no++ ) {
                // position the file pointer at the start of data to be retrieved
                fseek(this->f, this->header->get_data_start_offset() + (this->header->get_record_length() * rec_no), SEEK_SET);
                fread(&rec_status, sizeof(unsigned char), 1, this->f);
                fread(&rec_ptr,    sizeof(unsigned int),  1, this->f);

                cout << (rec_no + 1) << ";";
                for ( i=0; i<this->header->get_total_fields(); i++ ) {
                    // find out what type of field it is and display its contents
                    // based on its type
                    if ( this->fields.at(i)->get_field_type() == 3 ) { // if field is of type 3 (STRING)
                        fld_string = (char *)malloc(sizeof(char) * this->fields.at(i)->get_field_length());
                        fread(fld_string, sizeof(char), this->fields.at(i)->get_field_length(), this->f);
                        cout << fld_string;
                        free(fld_string);
                    } else if ( this->fields.at(i)->get_field_type() == 6 ) { // if field is of type 6 (SHORT)
                        if ( this->fields.at(i)->arrnum != 0 ) {
                            for ( j=0; j<(this->fields.at(i)->length / sizeof(unsigned short))-1; j++ ) {
                                fread(&fld_short, sizeof(unsigned short), 1, this->f);
                                cout << fld_short << ",";
                            }
                            fread(&fld_short, sizeof(unsigned short), 1, this->f);
                            cout << fld_short;
                        } else {
                            fread(&fld_short, sizeof(unsigned short), 1, this->f);
                            cout << fld_short;
                        }
                    } else {
                        cout << "*field_not_recognized*";
                        fseek(this->f, ftell(this->f)+this->fields.at(i)->get_field_length(), SEEK_SET);
                    }
                    cout << ";";
                }
                cout << endl;
            }
        }

        // retrieve the rec_no'th record of .dat file
        void get_record(unsigned int rec_no) {
            // record header definition
            unsigned char rec_status; // record header type and status
            unsigned int  rec_ptr;    // point to next deleted record or memo if active

            unsigned int i, j;

            // variables
            char *fld_string;
            unsigned short fld_short;

            if ( rec_no < 0 || rec_no >= this->header->get_total_records() ) {
                cout << "Error (get_record): invalid record number!" << endl;
            } else {
                // position the file pointer at the start of data to be retrieved
                fseek(this->f, this->header->get_data_start_offset() + (this->header->get_record_length() * rec_no), SEEK_SET);
                fread(&rec_status, sizeof(unsigned char), 1, this->f);
                fread(&rec_ptr,    sizeof(unsigned int),  1, this->f);
                //cout << "record status: 0x" << hex << rec_status << endl;
                //cout << "record pointer: 0x" << hex << rec_ptr << endl;

                cout << "RECORD_ID       : " << rec_no + 1 << endl;
                for ( i=0; i<this->header->get_total_fields(); i++ ) {
                    cout << this->fields.at(i)->get_field_name() << ": ";
                    // find out what type of field it is and display its contents
                    // based on its type
                    if ( this->fields.at(i)->get_field_type() == 3 ) { // if field is of type 3 (STRING)
                        fld_string = (char *)malloc(sizeof(char) * this->fields.at(i)->get_field_length());
                        fread(fld_string, sizeof(char), this->fields.at(i)->get_field_length(), this->f);
                        cout << "'" << fld_string << "'";
                        free(fld_string);
                    } else if ( this->fields.at(i)->get_field_type() == 6 ) { // if field is of type 6 (SHORT)
                        if ( this->fields.at(i)->arrnum != 0 ) {
                            for ( j=0; j<this->fields.at(i)->length / sizeof(unsigned short); j++ ) {
                                fread(&fld_short, sizeof(unsigned short), 1, this->f);
                                cout << "[" << fld_short << "]";
                            }
                        } else {
                            fread(&fld_short, sizeof(unsigned short), 1, this->f);
                            cout << fld_short;
                        }
                    } else {
                        cout << "*field_not_recognized*";
                        fseek(this->f, ftell(this->f)+this->fields.at(i)->get_field_length(), SEEK_SET);
                    }
                    cout << endl;
                }
            }
                
        }

        // t_dat_file class destructor
        ~t_dat_file() {
            fclose(this->f);
        }
};


