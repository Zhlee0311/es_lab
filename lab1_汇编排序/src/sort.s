		AREA DATA, DATA, READWRITE
		
numbers DCD 0382840771 0115524786 0943218196 1545101338 1875782096
		DCD 0028073711 0002598570 0079943281 0105045858 0209491162
			
length  EQU 10 ; 数组长度
index   EQU 9  ; 下标范围		
		
		AREA SORT, CODE
		EXPORT __main
		ENTRY
		
__main
        LDR R0, =numbers ; R0 = &numbers[0]
        MOV R1, #length  ; R1 = 10
        MOV R2, #index   ; R2 = 9

        ; 将立即数存储到numbers数组中
        LDR R3, =382840771
        STR R3, [R0, #0]
        LDR R3, =670542262
        STR R3, [R0, #4]
        LDR R3, =943218196
        STR R3, [R0, #8]
        LDR R3, =1545101338
        STR R3, [R0, #12]
        LDR R3, =1875782096
        STR R3, [R0, #16]
        LDR R3, =28073711
        STR R3, [R0, #20]
        LDR R3, =54560764
        STR R3, [R0, #24]
        LDR R3, =79943281
        STR R3, [R0, #28]
        LDR R3, =105045858
        STR R3, [R0, #32]
        LDR R3, =209491162
        STR R3, [R0, #36]

        BL  sort         ; 调用sort函数

done    
        B   done


;------------------------------------------------
;sort函数
;------------------------------------------------
sort 
        PUSH {R4, R5, R6, R7, LR}  ; 保存寄存器和返回地址
        MOV  R4, #0                ; R4 用作外层循环索引 i
        
outer_loop
        MOV  R5, #0                ; R5 用作内层循环索引 j

inner_loop
        LDR R6, [R0, R5, LSL #2]   ; R6 = numbers[j]
        ADD R5, R5, #1 
        LDR R7, [R0, R5, LSL #2]   ; R7 = numbers[j+1]
        SUB R5, R5, #1

        CMP R6, R7                 ; 比较numbers[j]和numbers[j+1]
        BLE no_swap                ; 如果numbers[j] <= numbers[j+1]，则不交换

        STR R7, [R0, R5, LSL #2]   ; swap(numbers[j], numbers[j+1])
        LDR R8, [R0, R5, LSL #2]
		ADD R5, R5, #1 
        STR R6, [R0, R5, LSL #2]
        SUB R5, R5, #1

no_swap
        ADD R5, R5, #1             ; j++
        CMP R5, R2                 ; 检查是否到达数组末尾
        BLT inner_loop             ; 如果没有到达数组末尾，则继续内层循环

        ADD R4, R4, #1             ; i++
        CMP R4, R1                 ; 检查是否遍历完
        BLT outer_loop             ; 若未遍历完，则继续外层循环

        POP {R4, R5, R6, R7, LR}
        BX  LR

        END