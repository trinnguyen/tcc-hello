import kotlinx.cinterop.CPointer
import parser.Parser
import platform.posix.*
import scanner.Scanner

/**
 * Build toy compiler for C.
 *
 * Sample grammar: https://github.com/antlr/grammars-v4/blob/master/c/C.g4
 */
fun main(args: Array<String>) {
    var name : String? = args.firstOrNull()

    // debug
    name = "samples/test1.c"

    // validate
    if (name == null) {
        println("error: missing input file")
        return
    }

    // get file
    val fin : CPointer<FILE>? = fopen(name, "r")
    if (fin == null) {
        println("error: can not open file $name")
        return
    }

    // create scanner
    val scanner = Scanner(fin)
    val parser = Parser(scanner)

    println(parser.parse())

    // close file
    fclose(fin)
}