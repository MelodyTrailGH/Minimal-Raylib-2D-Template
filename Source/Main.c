#include "raylib.h"
#include <stdio.h>
#include <sys/_types/_null.h>
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define PLAYER_START_POSITION                                                                                                  \
    CLITERAL( Vector2 ) {                                                                                                      \
        0.0f, 0.0f                                                                                                             \
    }

const char *get_resource_dir( );

#define TILE_NONE 0
#define TILE_TOPLEFT 1
#define TILE_TOPCENTER 2
#define TILE_TOPRIGHT 3
#define TILE_MIDDLELEFT 4
#define TILE_MIDDLECENTER 5
#define TILE_MIDDLERIGHT 6
#define TILE_BOTTOMLEFT 7
#define TILE_BOTTOMCENTER 8
#define TILE_BOTTOMRIGHT 9

#define VIEWPORT_WIDTH 320
#define VIEWPORT_HEIGHT 200

#define MAX( a, b ) ( ( a ) > ( b ) ? ( a ) : ( b ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )

typedef struct Player {
    Vector2 position;
    Texture2D texture;
} Player;

void player_init( Player *player );
void player_remove( Player *player );
void player_draw( const Player *player );

Image GenerateTilemap( const Vector2 tilemap_size, const unsigned int tilemap[( int )tilemap_size.x][( int )tilemap_size.y],
                       Image *tileset, const Vector2 tile_size );

int main( ) {
    InitWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Raylib Window" );
    Player player;
    player_init( &player );

    Image tileset = LoadImage( TextFormat( "%s/grassland.png", get_resource_dir( ) ) );
    const unsigned int map[8][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 0 }, //
        { 0, 0, 0, 0, 0, 0, 0, 0 }, //
        { 0, 0, 0, 0, 0, 0, 0, 0 }, //
        { 0, 0, 0, 0, 0, 0, 0, 0 }, //
        { 0, 0, 0, 0, 0, 0, 0, 0 }, //
        { 1, 2, 2, 2, 2, 2, 2, 3 }, //
        { 4, 5, 5, 5, 5, 5, 5, 6 }, //
        { 7, 8, 8, 8, 8, 8, 8, 9 }, //
    };
    Image tilemap_img = GenerateTilemap( ( Vector2 ){ 8, 8 }, map, &tileset, ( Vector2 ){ 16, 16 } );
    Texture2D tilemap = LoadTextureFromImage( tilemap_img );
    RenderTexture2D frame_buffer = LoadRenderTexture( 320, 200 );
    while ( !WindowShouldClose( ) ) {
        BeginTextureMode( frame_buffer );
        ClearBackground( RED );
        DrawTexture( tilemap, 0, 0, WHITE );
        player_draw( &player );
        EndTextureMode( );

        BeginDrawing( );
        ClearBackground( BLACK ); // Clear screen background
        float renderScale = MIN( ( float )GetScreenWidth( ) / VIEWPORT_WIDTH, ( float )GetScreenHeight( ) / VIEWPORT_HEIGHT );
        DrawTexturePro( frame_buffer.texture,
                        ( Rectangle ){ 0.0f, 0.0f, ( float )frame_buffer.texture.width, ( float )-frame_buffer.texture.height },
                        ( Rectangle ){ ( ( float )GetScreenWidth( ) - ( ( float )VIEWPORT_WIDTH * renderScale ) ) * 0.5f,
                                       ( ( float )GetScreenHeight( ) - ( ( float )VIEWPORT_HEIGHT * renderScale ) ) * 0.5f,
                                       ( float )VIEWPORT_WIDTH * renderScale, ( float )VIEWPORT_HEIGHT * renderScale },
                        ( Vector2 ){ 0, 0 }, 0.0f, WHITE );
        EndDrawing( );
    }
    player_remove( &player );
    UnloadImage( tileset );
    UnloadImage( tilemap_img );
}

const char *get_resource_dir( ) {
#if defined( STANDALONE )
    return TextFormat( "%sResources", GetApplicationDirectory( ) );
#elif defined( __APPLE__ )
#include <TargetConditionals.h>
#if defined( TARGET_OS_MAC )
    return TextFormat( "%s../Resources", GetApplicationDirectory( ) );
#endif
#endif
}

void player_init( Player *player ) {
    player->position = PLAYER_START_POSITION;
    player->texture = LoadTexture( TextFormat( "%s/mark.png", get_resource_dir( ) ) );
}

void player_remove( Player *player ) {
    UnloadTexture( player->texture );
}

void player_draw( const Player *player ) {
    DrawTextureV( player->texture, player->position, WHITE );
}

Image GenerateTilemap( const Vector2 tilemap_size, const unsigned int tilemap[( int )tilemap_size.x][( int )tilemap_size.y],
                       Image *tileset, const Vector2 tile_size ) {

    Image image = GenImageColor( ( int )( tilemap_size.x * tile_size.x ), ( int )( tilemap_size.y * tile_size.y ), BLANK );
    for ( int i = 0; i < ( int )tilemap_size.y; i++ ) {
        for ( int j = 0; j < ( int )tilemap_size.x; j++ ) {

            Image tile;
            switch ( tilemap[i][j] ) {

            case TILE_TOPLEFT:
                tile = ImageFromImage( *tileset, ( Rectangle ){ 0.0f, 0.0f, tile_size.x, tile_size.y } );
                break;

            case TILE_TOPCENTER:
                tile = ImageFromImage( *tileset, ( Rectangle ){ tile_size.x, 0.0f, tile_size.x, tile_size.y } );
                break;

            case TILE_TOPRIGHT:
                tile = ImageFromImage( *tileset, ( Rectangle ){ tile_size.x * 2, 0.0f, tile_size.x, tile_size.y } );
                break;

            case TILE_MIDDLELEFT:
                tile = ImageFromImage( *tileset, ( Rectangle ){ 0.0f, tile_size.y, tile_size.x, tile_size.y } );
                break;

            case TILE_MIDDLECENTER:
                tile = ImageFromImage( *tileset, ( Rectangle ){ tile_size.x, tile_size.y, tile_size.x, tile_size.y } );
                break;

            case TILE_MIDDLERIGHT:
                tile = ImageFromImage( *tileset, ( Rectangle ){ tile_size.x * 2, tile_size.y, tile_size.x, tile_size.y } );
                break;

            case TILE_BOTTOMLEFT:
                tile = ImageFromImage( *tileset, ( Rectangle ){ 0.0f, tile_size.y * 2, tile_size.x, tile_size.y } );
                break;

            case TILE_BOTTOMCENTER:
                tile = ImageFromImage( *tileset, ( Rectangle ){ tile_size.x, tile_size.y * 2, tile_size.x, tile_size.y } );
                break;

            case TILE_BOTTOMRIGHT:
                tile = ImageFromImage( *tileset, ( Rectangle ){ tile_size.x * 2, tile_size.y * 2, tile_size.x, tile_size.y } );
                break;

            default:
                continue;
            }
            ImageDraw( &image, tile, ( Rectangle ){ 0.0f, 0.0f, tile_size.x, tile_size.y },
                       ( Rectangle ){ tilemap_size.x * j, tilemap_size.y * i, tile_size.x, tile_size.y }, WHITE );
        }
    }
    return image;
}