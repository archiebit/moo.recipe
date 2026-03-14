// Project.
#include <moo/lex.hh>
#include <moo/lexeme.hh>
#include <moo/error.hh>

// STD stuff.
#include <fstream>
#include <filesystem>




namespace moo
{
    extern std::u32string source_full;
}


namespace moo
{
    static std::u32string read_file( std::string const & path )
    {
        std::ifstream file( path, std::ios::binary );
        std::u8string data;
        
        
        if( not file.is_open( ) )
        {
            throw error( "Can not open recipe file." );
        }


        // Set proper size.
        file.seekg( 0, std::ios::end ); data.append( file.tellg( ), u8' ' );
        file.seekg( 0, std::ios::beg );

        file.read( reinterpret_cast<char *>( data.data( ) ), data.size( ) * sizeof( char8_t ) );


        // UTF-8 to UTF-32 conversion. I know that it looks stupid.
        return std::filesystem::path( data ).u32string( );
    }
}


namespace moo
{
    // Finite State Machine.
    class machine
    {
    public:
        stream process( std::u32string const & code );


    private:
        using state = void( machine::* )( );


        void       root( );
        void       exit( );
        void identifier( );
        void    keyword( );
        void     string( );
        void whitespace( );
        void    newline( );
        void     square( );
        void     assign( );
        void       same( );
        void       diff( );
        void    comment( );


    private:
        void change( state value );
        void ignore( );
        void stride( );
        void extend( );
        void supply( );


    private:
        std::vector<token>  tokens;
        std::u32string_view source;
        std::size_t         offset;
        std::u32string      buffer;

        token_type  option;
        std::size_t   line;
        std::size_t column;
        std::size_t length;

        state current;
    };




    stream machine::process( std::u32string const & code )
    {
        // Reset internal details.
        source = code;
        offset = 0;

        tokens.clear( );
        buffer.clear( );

        option = NONE;
        line   = 1;
        column = 1;
        length = 0;

        current = & machine::root;


        // Run and change states.
        while( current )
        {
            ( this->*current )( );
        }


        return std::move( tokens );
    }




    void machine::root( )
    {
        auto letter = source[ offset ];


        // End of the source code?
        {
            if( offset == source.size( ) )
            {
                return change( & machine::exit );
            }
        }


        // Is identifier of a keyword?
        {
            bool cap = letter >= U'A' and letter <= U'Z';
            bool low = letter >= U'a' and letter <= U'z';
            bool und = letter == U'_';


            if( cap or low or und )
            {
                return change( & machine::identifier );
            }
        }


        // Is string literal?
        {
            if( letter == U'\"' )
            {
                return change( & machine::string );
            }
        }


        // Is whitespace?
        {
            bool suitable
            {
                letter == U' '  or
                letter == U'\f' or
                letter == U'\r' or
                letter == U'\t' or
                letter == U'\v'
            };


            if( suitable )
            {
                return change( & machine::whitespace );
            }
        }


        // Is new line?
        {
            if( letter == U'\n' )
            {
                return change( & machine::newline );
            }
        }


        // Is square bracket?
        {
            if( letter == U'[' or letter == U']' )
            {
                return change( & machine::square );
            }
        }


        // Is assign or '==' command?
        {
            if( letter == U'=' )
            {
                return change( & machine::assign );
            }
        }


        // Is '!=' command?
        {
            if( letter == U'!' )
            {
                return change( & machine::diff );
            }
        }


        // Is it comment?
        {
            if( letter == U'#' or letter == U';' )
            {
                return change( & machine::comment );
            }
        }
    }


    void machine::exit( )
    {
        option = END;

        supply( );

        return change( nullptr );
    }


    void machine::identifier( )
    {
        buffer.data( );
        auto letter = source[ offset ];


        // Init.
        if( length == 0 )
        {
            option = ID;

            return extend( );
        }


        // Possible values.
        {
            bool cap = letter >= U'A' and letter <= U'Z';
            bool low = letter >= U'a' and letter <= U'z';
            bool und = letter == U'_';


            if( cap or low or und )
            {
                return extend( );
            }
        }


        // Non-possible character in the source code. Must change the state.
        // Keyword check.
        {
            bool suitable
            {
                buffer == U"recipe" or
                buffer == U"config" or
                buffer == U"prepare"
            };


            if( suitable )
            {
                return change( & machine::keyword );
            }
        }


        supply( );

        return change( & machine::root );
    }


    void machine::keyword( )
    {
        if( buffer == U"recipe"  ) option = RECIPE;
        if( buffer == U"config"  ) option = CONFIG;
        if( buffer == U"prepare" ) option = PREPARE;

        supply( );

        return change( & machine::root );
    }


    void machine::string( )
    {
        auto letter = source[ offset ];


        // Init.
        if( length == 0 )
        {
            option = STRING;

            return extend( );
        }


        // End of the source code.
        if( letter == U'\n' )
        {
            throw error( "String literal must be enclosed by quots.", { { line, column, length } } );
        }


        // End of the string.
        if( letter == U'\"' )
        {
            extend( );
            supply( );

            return change( & machine::root );
        }


        // Extend string.
        return extend( );
    }


    void machine::whitespace( )
    {
        ignore( );

        return change( & machine::root );
    }


    void machine::newline( )
    {
        option = LINE;

        extend( );
        supply( );
        stride( );

        return change( & machine::root );
    }


    void machine::square( )
    {
        if( source[ offset ] == U'[' )
        {
            option = SQUARE_OPENED;

            extend( );
            supply( );

            return change( & machine::root );
        }


        if( source[ offset ] == U']' )
        {
            option = SQUARE_CLOSED;

            extend( );
            supply( );

            return change( & machine::root );
        }
    }


    void machine::assign( )
    {
        auto letter = source[ offset ];


        // Init.
        if( length == 0 )
        {
            option = ASSIGN;

            return extend( );
        }


        // Operator '=='?
        {
            if( letter == U'=' )
            {
                return change( & machine::same );
            }
        }


        // Fallback. Just 'assign' token.
        {
            supply( );

            return change( & machine::root );
        }
    }


    void machine::same( )
    {
        option = SAME;

        extend( );
        supply( );

        return change( & machine::root );
    }


    void machine::diff( )
    {
        auto letter = source[ offset ];


        // Init.
        if( length == 0 )
        {
            option = DIFF;

            return extend( );
        }


        // Operator '!='?
        {
            if( letter == U'=' )
            {
                extend( );
                supply( );

                return change( & machine::root );
            }
        }


        // Oops! Invalid token.
        {
            throw error( "Invalid token. Did you mean \'!=\' operator?", { { line, column, length } } );
        }
    }


    void machine::comment( )
    {
        auto letter = source[ offset ];


        if( length == 0 )
        {
            option = COMMENT;

            return extend( );
        }


        if( letter != U'\n' and offset != source.size( ) - 1 )
        {
            return extend( );
        }
        else
        {
            supply( );

            return change( & machine::root );
        }
    }




    void machine::change( state value )
    {
        current = value;
    }


    void machine::ignore( )
    {
        option = NONE;

        if( length != 0 )
        {
            column += length;
            length  = 0;
        }
        else
        {
            column += 1;
            offset += 1;
        }
    }


    void machine::stride( )
    {
        option = NONE;
        line  += 1;
        column = 1;
        length = 0;
    }


    void machine::extend( )
    {
        buffer += source[ offset ];

        offset += 1;
        length += 1;
    }


    void machine::supply( )
    {
        tokens.emplace_back( option, line, column, length );


        option  = NONE;
        column += length;
        length  = 0;

        buffer.clear( );
    }
}


namespace moo
{
    stream parse( std::string const & path )
    {
        source_full = read_file( path );

        machine fst;

        return fst.process( source_full );
    }
}