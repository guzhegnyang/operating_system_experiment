00007C00  8CC8              mov ax,cs
00007C02  8ED8              mov ds,ax
00007C04  8EC0              mov es,ax
00007C06  8ED0              mov ss,ax
00007C08  E81800            call 0x7c23
00007C0B  BDD47C            mov bp,0x7cd4
00007C0E  8CD8              mov ax,ds
00007C10  8EC0              mov es,ax
00007C12  B90A00            mov cx,0xa
00007C15  B80113            mov ax,0x1301
00007C18  BB0700            mov bx,0x7
00007C1B  B60A              mov dh,0xa
00007C1D  B20A              mov dl,0xa
00007C1F  CD10              int 0x10
00007C21  EBFE              jmp short 0x7c21
00007C23  6655              push ebp
00007C25  6689E5            mov ebp,esp
00007C28  6683EC04          sub esp,byte +0x4
00007C2C  6766C745FC000000  mov dword [ebp-0x4],0x0
         -00
00007C35  EB4B              jmp short 0x7c82
00007C37  67668B45FC        mov eax,[ebp-0x4]
00007C3C  6605D47C0000      add eax,0x7cd4
00007C42  678A00            mov al,[eax]
00007C45  3C60              cmp al,0x60
00007C47  7E34              jng 0x7c7d
00007C49  67668B45FC        mov eax,[ebp-0x4]
00007C4E  6605D47C0000      add eax,0x7cd4
00007C54  678A00            mov al,[eax]
00007C57  3C7A              cmp al,0x7a
00007C59  7F22              jg 0x7c7d
00007C5B  67668B45FC        mov eax,[ebp-0x4]
00007C60  6605D47C0000      add eax,0x7cd4
00007C66  678A00            mov al,[eax]
00007C69  6683E820          sub eax,byte +0x20
00007C6D  88C2              mov dl,al
00007C6F  67668B45FC        mov eax,[ebp-0x4]
00007C74  6605D47C0000      add eax,0x7cd4
00007C7A  678810            mov [eax],dl
00007C7D  6766FF45FC        inc dword [ebp-0x4]
00007C82  67668B45FC        mov eax,[ebp-0x4]
00007C87  6605D47C0000      add eax,0x7cd4
00007C8D  678A00            mov al,[eax]
00007C90  84C0              test al,al
00007C92  75A3              jnz 0x7c37
00007C94  90                nop
00007C95  66C9              o32 leave
00007C97  66C3              retd
00007C99  0000              add [bx+si],al
00007C9B  0014              add [si],dl
00007C9D  0000              add [bx+si],al
00007C9F  0000              add [bx+si],al
00007CA1  0000              add [bx+si],al
00007CA3  0001              add [bx+di],al
00007CA5  7A52              jpe 0x7cf9
00007CA7  0001              add [bx+di],al
00007CA9  7C08              jl 0x7cb3
00007CAB  011B              add [bp+di],bx
00007CAD  0C04              or al,0x4
00007CAF  0488              add al,0x88
00007CB1  0100              add [bx+si],ax
00007CB3  001C              add [si],bl
00007CB5  0000              add [bx+si],al
00007CB7  001C              add [si],bl
00007CB9  0000              add [bx+si],al
00007CBB  0067FF            add [bx-0x1],ah
00007CBE  FF                db 0xff
00007CBF  FF7600            push word [bp+0x0]
00007CC2  0000              add [bx+si],al
00007CC4  00420E            add [bp+si+0xe],al
00007CC7  08850243          or [di+0x4302],al
00007CCB  0D0502            or ax,0x205
00007CCE  6F                outsw
00007CCF  C50C              lds cx,[si]
00007CD1  0404              add al,0x4
00007CD3  004161            add [bx+di+0x61],al
00007CD6  42                inc dx
00007CD7  624363            bound ax,[bp+di+0x63]
00007CDA  44                inc sp
00007CDB  6445              fs inc bp
00007CDD  65                gs
