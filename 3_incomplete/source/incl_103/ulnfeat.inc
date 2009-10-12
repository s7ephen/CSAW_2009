ifndef ??version
.err <This features required TASM>
elseif ??version LT 500h
err 'This features required TASM 5.0 or higher'
elseifndef @Model
err 'MODEL must be first'
elseif @Model NE 1
err 'Required FLAT model'
elseifndef __ulnk_feat_inc__
__ulnk_feat_ink = 1

;-----------------------------------------------------------------------
TLS_CALLBACK macro nid, fnc
_TEXT segment
_TLS_CALLBACK_&nid segment virtual
_&nid label near
        dd      offset fnc
ends
ends
endm

;-----------------------------------------------------------------------
ifndef RESOURCE_STRING_NOSMART
RESOURCE_STRINGH_NOSMART = 1
endif

_RESOURCE_STRING_ID macro n
extrn _RESOURCE_STRING_&n : Ptr
endm

PUSH_RESOURCE_STRING_ID macro n
_RESOURCE_STRING_ID n
    push offset _RESOURCE_STRING_&n
endm

GET_RESOURCE_STRING_ID macro arg, n
_RESOURCE_STRING_ID n
    mov  arg, offset _RESOURCE_STRING_&n
endm


RESOURCE_STRING macro n, l, s, t
local ??l1, ??l2, ??l3, ??l4

_RESOURCE_STRING_ID n

??l3 CATSTR <_>,<&n>,<_>

_tonum macro val
??l3 CATSTR ??l3,<&val>
endm

pushstate
radix 16
 _tonum %&l
??l3 CATSTR ??l3,<_>
 _tonum %&s
popstate

??l4 CATSTR <_RESOURCE_STRING>, ??l3

_TEXT segment
??l4 segment virtual
??l3 label near
ife RESOURCE_STRING_NOSMART
        dw      0
else
        dw      2
endif
        dw      ??l2 - ??l1
??l1 label near
        db      t, 0
??l2 label near
ends
ends

endm

;-----------------------------------------------------------------------
endif ; __ulnk_feat_ink__
