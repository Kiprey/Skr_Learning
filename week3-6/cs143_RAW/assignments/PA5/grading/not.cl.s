# start of generated code
	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
_int_tag:
	.word	2
_bool_tag:
	.word	3
_string_tag:
	.word	4
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const8:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const3
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const4
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const3
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const5
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const6
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	4
	.word	6
	.word	String_dispTab
	.word	int_const4
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	4
	.word	8
	.word	String_dispTab
	.word	int_const7
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	4
	.word	7
	.word	String_dispTab
	.word	int_const8
	.ascii	"./not.cl"
	.byte	0	
	.align	2
	.word	-1
int_const8:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	8
	.word	-1
int_const7:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	13
	.word	-1
int_const6:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	2
	.word	-1
int_const5:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	3
	.word	-1
int_const4:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	6
	.word	-1
int_const3:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	4
	.word	-1
int_const2:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	1
	.word	-1
int_const1:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	100
	.word	-1
int_const0:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
bool_const0:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
bool_const1:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	1
class_nameTab:
	.word	str_const2
	.word	str_const3
	.word	str_const4
	.word	str_const5
	.word	str_const6
	.word	str_const7
class_objTab:
	.word	Object_protObj
	.word	Object_init
	.word	IO_protObj
	.word	IO_init
	.word	Int_protObj
	.word	Int_init
	.word	Bool_protObj
	.word	Bool_init
	.word	String_protObj
	.word	String_init
	.word	Main_protObj
	.word	Main_init
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
IO_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
Int_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Bool_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.length
	.word	String.concat
	.word	String.substr
Main_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Main.main
	.word	-1
Object_protObj:
	.word	0
	.word	3
	.word	Object_dispTab
	.word	-1
IO_protObj:
	.word	1
	.word	3
	.word	IO_dispTab
	.word	-1
Int_protObj:
	.word	2
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
Bool_protObj:
	.word	3
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
String_protObj:
	.word	4
	.word	5
	.word	String_dispTab
	.word	int_const0
	.word	0
	.word	-1
Main_protObj:
	.word	5
	.word	3
	.word	Main_dispTab
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
Object_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
IO_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Int_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Bool_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
String_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main_init:
	addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main.main:
	addiu	$sp $sp -24
	sw	$fp 24($sp)
	sw	$s0 20($sp)
	sw	$ra 16($sp)
	addiu	$fp $sp 4
	move	$s0 $a0
	sw	$s1 8($fp)
	sw	$s2 4($fp)
	sw	$s3 0($fp)
	la	$s3 int_const0
	la	$s2 bool_const0
label0:
	move	$s1 $s3
	la	$a0 int_const1
	lw	$t1 12($s1)
	lw	$t2 12($a0)
	la	$a0 bool_const1
	blt	$t1 $t2 label2
	la	$a0 bool_const0
label2:
	lw	$t1 12($a0)
	beq	$t1 $zero label1
	move	$s1 $s2
	lw	$t1 12($s2)
	la	$a0 bool_const1
	beqz	$t1 label14
	la	$a0 bool_const0
label14:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label13
	la	$a0 bool_const0
label13:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label12
	la	$a0 bool_const0
label12:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label11
	la	$a0 bool_const0
label11:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label10
	la	$a0 bool_const0
label10:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label9
	la	$a0 bool_const0
label9:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label8
	la	$a0 bool_const0
label8:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label7
	la	$a0 bool_const0
label7:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label6
	la	$a0 bool_const0
label6:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label5
	la	$a0 bool_const0
label5:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label4
	la	$a0 bool_const0
label4:
	move	$t1 $s1
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label3
	la	$a1 bool_const0
	jal	equality_test
label3:
	move	$s1 $s2
	lw	$t1 12($s2)
	la	$a0 bool_const1
	beqz	$t1 label26
	la	$a0 bool_const0
label26:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label25
	la	$a0 bool_const0
label25:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label24
	la	$a0 bool_const0
label24:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label23
	la	$a0 bool_const0
label23:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label22
	la	$a0 bool_const0
label22:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label21
	la	$a0 bool_const0
label21:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label20
	la	$a0 bool_const0
label20:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label19
	la	$a0 bool_const0
label19:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label18
	la	$a0 bool_const0
label18:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label17
	la	$a0 bool_const0
label17:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label16
	la	$a0 bool_const0
label16:
	move	$t1 $s1
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label15
	la	$a1 bool_const0
	jal	equality_test
label15:
	move	$s1 $s2
	lw	$t1 12($s2)
	la	$a0 bool_const1
	beqz	$t1 label38
	la	$a0 bool_const0
label38:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label37
	la	$a0 bool_const0
label37:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label36
	la	$a0 bool_const0
label36:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label35
	la	$a0 bool_const0
label35:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label34
	la	$a0 bool_const0
label34:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label33
	la	$a0 bool_const0
label33:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label32
	la	$a0 bool_const0
label32:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label31
	la	$a0 bool_const0
label31:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label30
	la	$a0 bool_const0
label30:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label29
	la	$a0 bool_const0
label29:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label28
	la	$a0 bool_const0
label28:
	move	$t1 $s1
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label27
	la	$a1 bool_const0
	jal	equality_test
label27:
	move	$s1 $s2
	lw	$t1 12($s2)
	la	$a0 bool_const1
	beqz	$t1 label50
	la	$a0 bool_const0
label50:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label49
	la	$a0 bool_const0
label49:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label48
	la	$a0 bool_const0
label48:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label47
	la	$a0 bool_const0
label47:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label46
	la	$a0 bool_const0
label46:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label45
	la	$a0 bool_const0
label45:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label44
	la	$a0 bool_const0
label44:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label43
	la	$a0 bool_const0
label43:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label42
	la	$a0 bool_const0
label42:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label41
	la	$a0 bool_const0
label41:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label40
	la	$a0 bool_const0
label40:
	move	$t1 $s1
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label39
	la	$a1 bool_const0
	jal	equality_test
label39:
	move	$s1 $s2
	lw	$t1 12($s2)
	la	$a0 bool_const1
	beqz	$t1 label62
	la	$a0 bool_const0
label62:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label61
	la	$a0 bool_const0
label61:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label60
	la	$a0 bool_const0
label60:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label59
	la	$a0 bool_const0
label59:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label58
	la	$a0 bool_const0
label58:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label57
	la	$a0 bool_const0
label57:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label56
	la	$a0 bool_const0
label56:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label55
	la	$a0 bool_const0
label55:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label54
	la	$a0 bool_const0
label54:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label53
	la	$a0 bool_const0
label53:
	lw	$t1 12($a0)
	la	$a0 bool_const1
	beqz	$t1 label52
	la	$a0 bool_const0
label52:
	move	$t1 $s1
	move	$t2 $a0
	la	$a0 bool_const1
	beq	$t1 $t2 label51
	la	$a1 bool_const0
	jal	equality_test
label51:
	move	$s1 $s3
	la	$a0 int_const2
	jal	Object.copy
	lw	$t2 12($a0)
	lw	$t1 12($s1)
	add	$t1 $t1 $t2
	sw	$t1 12($a0)
	move	$s3 $a0
	b	label0
label1:
	move	$a0 $zero
	lw	$t1 12($s2)
	beqz	$t1 label63
	move	$a0 $s0
	bne	$a0 $zero label65
	la	$a0 str_const0
	li	$t1 18
	jal	_dispatch_abort
label65:
	lw	$t1 8($a0)
	lw	$t1 0($t1)
	jalr		$t1
	b	label64
label63:
	la	$a0 int_const0
label64:
	lw	$s1 8($fp)
	lw	$s2 4($fp)
	lw	$s3 0($fp)
	lw	$fp 24($sp)
	lw	$s0 20($sp)
	lw	$ra 16($sp)
	addiu	$sp $sp 24
	jr	$ra	

# end of generated code
