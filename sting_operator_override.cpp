//Author Alexander Popov
//Here is example of '[]' operator overloading (Make for Stepik task)

#include <cstddef>
#include <iostream>
//Here we initilize the string object and its methods
struct String {
	//Helper function to resize the string
    char *resize( char *old_str, unsigned old_size, unsigned new_size) 
	{
		char * new_str = new char [new_size];
		unsigned min_size = old_size > new_size ? new_size : old_size;
		for (size_t i = 0; i < min_size;++i) 
		{
			*(new_str+i) = *(old_str+i);
		}
		delete [] old_str;
		return new_str;
	}  
	//Constructor
	String(const char *str = "") 
	{
        this->str = new char[1];
        for(size = 1; str[size-1] !='\0'; ++size)
        {
            this->str = resize(this->str, size, size+1);
            this->str[size-1] = str[size-1]; 
            this->str[size] = '\0';        
        }
        --size;
    }
	//Destructor
	~String()
	{  
        delete [] this->str;    
    }

	size_t size;
	char *str;
	
	//This describes a class for a substring, which will be further needed to overload the '[]' operator
    struct Substring {
		//Helper function to resize the string
        char *resize( char *old_str, unsigned old_size, unsigned new_size) 
        {
            char * new_str = new char [new_size];
            unsigned min_size = old_size > new_size ? new_size : old_size;
            for (size_t i = 0; i < min_size;++i) 
            {
                *(new_str+i) = *(old_str+i);
            }
            delete [] old_str;
            return new_str;
        }
		//Constructor
	    Substring(const char *tmp_substr = "", size_t const& first_index = 0) 
        {
            this->first_index = first_index;
            substr = new char[1];
            for(sub_size = 1; tmp_substr[sub_size-1] !='\0'; ++sub_size)
            {
                substr = resize(substr, sub_size, sub_size+1);
                *(substr+sub_size-1) = tmp_substr[sub_size-1]; 
                *(substr+sub_size) = '\0';        
            }
            --sub_size;
        }
		//Destructor
        ~Substring()   
        {
            delete [] this->substr;
        }
        size_t first_index;
    	size_t sub_size;
	    char *substr;        
		//Here the operator is overloaded with the return of a String class object
        String operator[](size_t const& j) const
		{
            char *tmp_string = new char[j-first_index+1];
            for (size_t i = 0; i < (j-first_index); i++){
                tmp_string[i] = substr[i];
            }
            tmp_string[j-first_index] = '\0';
            String str2(tmp_string);
            delete [] tmp_string;
            return str2;
        }
    };    
	//Here the operator is overloaded with the return of a Substring class object
    Substring operator[](size_t const& i) const 
	{
        char *tmp_string = new char[size+1-i];
        size_t k = 0;
        for (size_t j = i; j < size; j++){
            tmp_string[k] = str[j];
            ++k;
        }
        tmp_string[size-i] = '\0';
        Substring substr1(tmp_string, i);
        delete [] tmp_string;
        return substr1;
    }
};

int main() {
    String newstr("Hello World!");
    String s1 = newstr[3][8];
    std::cout << s1.str;
    return 0;
}