// STD stuff.
#include <iostream>

// Project.
#include <moo/error.hh>



// Global variables.
namespace moo
{
    // Source code.
    extern std::vector<std::u32string> source;
}


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
    // Vertical area around lexeme. Two lines up and two lines down.
    // For fancy printing.
    struct area
    {
        std::size_t line_beg;
        std::size_t line_end;


        static std::size_t const offset = 2;
    };


    static std::vector<area> compute_areas( std::vector<lexeme> const & lexemes )
    {
        using ssize = std::make_signed_t<std::size_t>;


        std::vector<area> output;


        ssize const line_min = 1;
        ssize const line_max = source.size( );

        for( auto const & element : lexemes )
        {
            ssize beg = element.get_line( );
            ssize end = element.get_line( );

            beg = ( beg >= line_min + area::offset ) ? beg - area::offset : line_min;
            end = ( end <= line_max - area::offset ) ? end + area::offset : line_max;


            output.emplace_back( beg, end );
        }


        return output;
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
        std::vector<area> areas = compute_areas( lexemes );
        // TODO: Implement error lexemes output.
    }
}