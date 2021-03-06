        [bits 64]

        %include "audio.inc"

        global  wolfasm_init_audio, wolfasm_deinit_audio,       \
        wolfasm_play_sound, wolfasm_menu_play_music

        ;; SDL functions
        extern  _Mix_OpenAudio, _Mix_CloseAudio, _SDL_GetError, \
        _SDL_RWFromFile, _Mix_LoadWAV_RW, _Mix_FreeChunk,       \
        _Mix_PlayChannelTimed, _Mix_LoadMUS, _Mix_FreeMusic,    \
        _Mix_PlayMusic

        ;; LibC functions
        extern _puts, _exit, _rand

        section .text

wolfasm_init_audio:
        push      rbp
        mov       rbp, rsp

        ;; Start SDL_Mixer
        mov       rdi, 22050                        ;; Frequency
        mov       rsi, 0x8010                       ;; Format
        mov       rdx, NB_WOLFASM_AUDIO_CHANNELS    ;; Channels
        mov       rcx, 4096                         ;; chunksize
        call      _Mix_OpenAudio

        cmp       rax, 0
        jl        .err

        ;; Load sounds
        xor       rcx, rcx
        mov       edx, NB_WOLFASM_SOUNDS
.loop_sounds:
        cmp       rcx, rdx
        je        .loop_sounds_end

        push      rcx   ;; Save counter
        push      rdx   ;; Save end

        ;; Load WAV sound file
        lea       rdi, [rel wolfasm_sound_file]
        mov       rdi, [rdi + rcx * 8]
        mov       rsi, wolfasm_sound_rb
        call      _SDL_RWFromFile

        ;; Check if returned NULL
        cmp       rax, 0
        je        .err

        mov       rdi, rax
        mov       rsi, 1
        call      _Mix_LoadWAV_RW

        pop       rdx   ;; Restore end
        pop       rcx   ;; Restore counter

        ;; Check if loaded
        cmp       rax, 0
        je        .err

        ;; Store sound
        lea       rdi, [rel wolfasm_sounds]
        mov       qword [rdi + rcx * 8], rax

        inc       rcx
        jmp       .loop_sounds
.loop_sounds_end:

        ;; Now load musics
        xor       rcx, rcx
        mov       edx, NB_WOLFASM_MUSICS
.loop_musics:
        cmp       ecx, edx
        je        .loop_musics_end

        push      rcx   ;; Save counter
        push      rdx   ;; Save end

        lea       rdi, [rel wolfasm_music_file]
        mov       rdi, [rdi + rcx * 8]
        call      _Mix_LoadMUS

        cmp       rax, 0
        je        .err

        pop       rdx   ;; Restore end
        pop       rcx   ;; Restore counter

        ;; Store music
        lea       rdi, [rel wolfasm_musics]
        mov       qword [rdi + rcx * 8], rax

        inc       rcx
        jmp       .loop_musics
.loop_musics_end:

        mov       rsp, rbp
        pop       rbp
        ret

        ;; Stop if error
.err:
        call      _SDL_GetError

        mov       rdi, rax
        call      _puts

        mov       rdi, 1
        call      _exit

wolfasm_deinit_audio:
        push      rbp
        mov       rbp, rsp

        xor        rcx, rcx
        mov        rdx, NB_WOLFASM_SOUNDS

.wolfasm_deinit_sounds_loop:
        cmp         rcx, rdx
        je          .wolfasm_deinit_sounds_loop_end

        push        rcx ;; Save counter
        push        rdx ;; Save end

        lea         rdx, [rel wolfasm_sounds]
        mov         rdi, [rdx + rcx * 8]
        mov         qword [rdx + rcx * 8], 0
        call        _Mix_FreeChunk

        pop         rdx ;; Restore counter
        pop         rcx ;; Restore end

        inc         rcx
        jmp         .wolfasm_deinit_sounds_loop
.wolfasm_deinit_sounds_loop_end:

        xor         rcx, rcx
        mov         edx, NB_WOLFASM_MUSICS
.wolfasm_deinit_musics_loop:
        cmp         ecx, edx
        je          .wolfasm_deinit_musics_loop_end

        push        rcx ;; Save counter
        push        rdx ;; Save end

        lea         rdi, [rel wolfasm_musics]
        mov         rdi, [rdi + rcx * 8]
        call        _Mix_FreeMusic

        pop         rdx ;; Restore counter
        pop         rcx ;; Restore end

        inc         rcx
        jmp         .wolfasm_deinit_musics_loop
.wolfasm_deinit_musics_loop_end:

        ;; Stop SDL_Mixer
        call      _Mix_CloseAudio

        mov       rsp, rbp
        pop       rbp
        ret

;; Play a sound
wolfasm_play_sound:
        push      rbp
        mov       rbp, rsp

.play:
        lea       rsi, [rel wolfasm_sounds]
        mov       rsi, [rsi + rdi * 8]
        mov       rdi, SOUND_CHANNEL
        mov       rdx,  0
        mov       rcx, -1
        call      _Mix_PlayChannelTimed

        cmp       rax, 0
        jge       .done
.err:
        call      _SDL_GetError
        mov       rdi, rax
        call      _puts
        mov       rdi, 1
        call      _exit

.done:
        mov       rsp, rbp
        pop       rbp
        ret

wolfasm_menu_play_music:
        push      rbp
        mov       rbp, rsp

        call      _rand
        xor       rdx, rdx
        mov       r8d, NB_WOLFASM_MUSIC_MENU
        div       r8d

        lea       rdi, [rel wolfasm_musics]
        mov       rdi, [rdi + rdx * 8]
        mov       rsi, 1
        call      _Mix_PlayMusic

        mov       rsp, rbp
        pop       rbp
        ret

        section .rodata
wolfasm_sound_rb:     db  "rb", 0x00

;; Sound files
wolfasm_sound_file_0: db  "./resources/sounds/pistol.wav", 0x00
wolfasm_sound_file_1: db  "./resources/sounds/shotgun.wav", 0x00
wolfasm_sound_file_2: db  "./resources/sounds/barrel.wav", 0x00
wolfasm_sound_file_3: db  "./resources/sounds/no_ammo.wav", 0x00
wolfasm_sound_file:   dq  wolfasm_sound_file_0,   \
                          wolfasm_sound_file_1,   \
                          wolfasm_sound_file_2,   \
                          wolfasm_sound_file_3

;; Music files
wolfasm_music_file_0: db   "./resources/sounds/menu_0.ogg", 0x00
wolfasm_music_file_1: db   "./resources/sounds/menu_1.ogg", 0x00
wolfasm_music_file_2: db   "./resources/sounds/menu_2.ogg", 0x00
wolfasm_music_file_3: db   "./resources/sounds/menu_3.ogg", 0x00
wolfasm_music_file:   dq    wolfasm_music_file_0,   \
                            wolfasm_music_file_1,   \
                            wolfasm_music_file_2,   \
                            wolfasm_music_file_3

        section .bss
wolfasm_sounds: resq  NB_WOLFASM_SOUNDS
wolfasm_musics: resq  NB_WOLFASM_MUSICS
