// Project.
#include <moo/lexeme.hh>




namespace moo
{
    lexeme::~lexeme( ) = default;


    lexeme::lexeme( std::size_t line, std::size_t column, std::size_t length )
    :     line( line )
    ,   column( column )
    ,   length( length )
    { }




    bool operator == ( lexeme const & a, lexeme const & b )
    {
        return
            ( a.line   == b.line   ) and
            ( a.column == b.column ) and
            ( a.length == b.length );
    }


    bool operator != ( lexeme const & a, lexeme const & b )
    {
        return
            ( a.line   != b.line   ) or
            ( a.column != b.column ) or
            ( a.length != b.length );
    }




    void lexeme::rewrite( std::size_t line, std::size_t column, std::size_t length )
    {
        this->line   = line;
        this->column = column;
        this->length = length;
    }




    lexeme::operator std::u32string_view( ) const
    {
        // TODO: Implement string view casting.
        return U"";
    }
}