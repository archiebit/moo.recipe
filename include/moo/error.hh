#ifndef MOO_ERROR_HH
#define MOO_ERROR_HH

// STD stuff.
#include <string_view>
#include <initializer_list>
#include <vector>

// Project.
#include <moo/lexeme.hh>




namespace moo
{
    class error
    {
    public:
       ~error( );
        error( std::string_view message );
        error( std::string_view message, std::initializer_list<lexeme> lexemes );


        void print( ) const;


    private:
        void print_message( ) const;
        void print_lexemes( ) const;


        std::string_view    message;
        std::vector<lexeme> lexemes;
    };
}




#endif