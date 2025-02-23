package main

import (
	"fmt"
	"os"
	"runtime"
	"sync"
	"syscall"
)

func main() {
	var wg sync.WaitGroup
	numGoroutines := 100000
	wg.Add(numGoroutines)

	// 创建一个通道来收集 Goroutine 的信息
	infoChan := make(chan string, numGoroutines)

	for i := 0; i < numGoroutines; i++ {
		go func(id int) {
			defer wg.Done()
			runtime.LockOSThread()
			defer runtime.UnlockOSThread()
			// 获取当前 Goroutine 的 ID
			gid := getGID()
			// 获取当前进程的 ID
			pid := getPID()
			// 获取当前线程的 ID
			tid := getTID()
			infoChan <- fmt.Sprintf("Goroutine %d: GID=%d , PID=%d , TID=%d", id, gid, pid, tid)
		}(i)
	}

	wg.Wait()
	close(infoChan)

	// 打印 Goroutine 的信息
	for info := range infoChan {
		fmt.Println(info)
	}
	// 获取 M 和 P 的数量
	// numCPU := runtime.NumCPU()
	// numGoroutine := runtime.NumGoroutine()
	// gomaxprocs := runtime.GOMAXPROCS(0)

	// fmt.Printf("Number of CPUs: %d\n", numCPU)
	// fmt.Printf("Number of Goroutines: %d\n", numGoroutine)
	// fmt.Printf("GOMAXPROCS: %d\n", gomaxprocs)
}

// 获取当前 Goroutine 的 ID
func getGID() int {
	var buf [64]byte
	n := runtime.Stack(buf[:], false)
	var gid int
	fmt.Sscanf(string(buf[:n]), "goroutine %d ", &gid)
	return gid
}

// 获取当前 Goroutine 的 pid
func getPID() int {
	return os.Getpid()
}
func getTID() int {
	return syscall.Gettid()
}
