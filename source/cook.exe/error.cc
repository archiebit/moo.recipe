// STD stuff.
#include <iostream>

// Project.
#include <moo/error.hh>




namespace moo
{
    error::~error( ) = default;


    error::error( std::string_view message )
    :   message( message )
    { }


    error::error( std::string_view message, std::initializer_list<lexeme> lexemes )
    :   message( message )
    ,   lexemes( lexemes )
    { }




    void error::print( ) const
    {
        print_message( );
        print_lexemes( );
    }




    void error::print_message( ) const
    {
        // TODO: Implement error message output.
    }


    void error::print_lexemes( ) const
    {
        // TODO: Implement error lexemes output.
    }
}