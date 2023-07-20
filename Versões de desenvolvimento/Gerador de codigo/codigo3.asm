mov eax, 3
sub eax, 1

mov ebx, 1
sub ebx, eax

mov ecx, 10
sub ecx, eax

sub edx, edx
push eax
mov eax, 10
mov esi, 2
idiv esi
mov esi, eax
pop eax
