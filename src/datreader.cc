#include <iostream>
#include <vector>
#include <iomanip> // setw

#include "../include/t_dat_file.h"

using namespace std;

int main(int argc, char *argv[]) {

    if ( argc < 2 ) {
        cout << "Too few parameterers." << endl;
        cout << " Use " << argv[0] << " <filename.dat>" << endl;
    } else {
        t_dat_file *dat = new t_dat_file(argv[1]);

        if ( dat->header != NULL ) {
            /*
            // show header data
            cout << "Keys: " << dat->header->get_total_keys() << 
                    " Fields: " << dat->header->get_total_fields() << 
                    " Pictures: " << dat->header->get_total_pictures() << 
                    " Arrays: " << dat->header->get_total_arrays() << 
                    " Records: " << dat->header->get_total_records() << endl;
            */

            /*
            // show fields data
            for ( i=0; i<dat->header->get_total_fields(); i++ ) {
                cout << "field " << setw(2) << i << " of " << dat->fields.size() << ": '" << dat->fields.at(i)->fldname << "' " <<
                        " type: " << (int)dat->fields.at(i)->get_field_type() << "-" << dat->fields.at(i)->get_field_type_desc() << 
                        " size: " << dat->fields.at(i)->get_field_length() <<
                        " is_array: " << dat->fields.at(i)->arrnum << endl;
            }
            */
          
            /*
            // show keys data
            for ( i=0; i<dat->header->get_total_keys(); i++ ) {
                cout << "Key " << i << ":" << endl;
                cout << "   components: " << (int)dat->keys.at(i)->numcomps << endl;
                cout << "   name: '" << dat->keys.at(i)->keyname << "'" << endl;
                for ( j=0; j<dat->keys.at(i)->keyparts.size(); j++ ) {
                    cout << "      Section " << j << ":" << endl;
                    cout << "         field type: " << (int)dat->keys.at(i)->keyparts.at(j)->fldtype << endl;
                }
            }
            */

            /*
            for ( i=0; i<dat->header->get_total_records(); i++ ) {
                dat->get_record(i);
            }
            */
                dat->get_records_as_table();
        }
    }
        
    return 0;
}
