// STD stuff.
#include <iostream>

// Project.
#include <moo/error.hh>




namespace moo
{
    // Fancy text formatting.
    enum format
    {
        RESET,

        BOLD,
        UNDERLINE,

        RED,
        WHITE,
        YELLOW,
        GRAY
    };


    std::ostream & operator << ( std::ostream & stream, format option )
    {
        // Using ANSI escape sequences.
        switch( option )
        {
            case RESET:     stream << "\x1B[0m";  break;

            case BOLD:      stream << "\x1B[1m";  break;
            case UNDERLINE: stream << "\x1B[4m";  break;

            case RED:       stream << "\x1B[31m"; break;
            case WHITE:     stream << "\x1B[37m"; break;
            case YELLOW:    stream << "\x1B[93m"; break;
            case GRAY:      stream << "\x1B[90m"; break;
        }


        return stream;
    }
}


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
        std::cerr << RESET;

        std::cerr << RED << "Error: " << UNDERLINE << WHITE << message;

        std::cerr << RESET << '\n';
    }


    void error::print_lexemes( ) const
    {
        // TODO: Implement error lexemes output.
    }
}