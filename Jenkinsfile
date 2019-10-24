#!groovy

pipeline {

  agent {
    label 'docker-gpu-host'
  }

  options {
    timeout(time: 1, unit: 'HOURS')
  }  

  stages {
    stage('Build') {
      parallel {
        stage('gcc-5') {
          agent {
            docker {
              reuseNode true
              image 'braintwister/ubuntu-18.04-cuda-10.1-cmake-3.15-gcc-5-conan-1.19'
              args '--runtime=nvidia'
            }
          }
          steps {
            sh './build.sh gcc-5 Release'
          }
          post {
            always {
              step([
                $class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false,
                defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '',
                parserConfigurations: [[parserName: 'GNU Make + GNU C Compiler (gcc)', pattern: 'build-gcc-5/make.out']],
                unHealthy: ''
              ])
            }
          }
        }
        stage('gcc-8') {
          agent {
            docker {
              reuseNode true
              image 'braintwister/ubuntu-18.04-cuda-10.1-cmake-3.15-gcc-8-conan-1.19'
              args '--runtime=nvidia'
            }
          }
          steps {
            sh './build.sh gcc-8 Release'
          }
          post {
            always {
              step([
                $class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false,
                defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '',
                parserConfigurations: [[parserName: 'GNU Make + GNU C Compiler (gcc)', pattern: 'build-gcc-8/make.out']],
                unHealthy: ''
              ])
            }
          }
        }
        stage('clang-6') {
          agent {
            docker {
              reuseNode true
              image 'braintwister/ubuntu-18.04-cuda-10.1-cmake-3.15-clang-6-conan-1.19'
              args '--runtime=nvidia'
            }
          }
          steps {
            sh './build.sh clang-6 Release'
          }
          post {
            always {
              step([
                $class: 'WarningsPublisher', canComputeNew: false, canResolveRelativePaths: false,
                defaultEncoding: '', excludePattern: '', healthy: '', includePattern: '', messagesPattern: '',
                parserConfigurations: [[parserName: 'Clang (LLVM based)', pattern: 'build-clang-6/make.out']],
                unHealthy: ''
              ])
            }
          }
        }
      }
    }
    stage('Test') {
      parallel {
        stage('gcc-5') {
          agent {
            docker {
              reuseNode true
              image 'braintwister/ubuntu-18.04-cuda-10.1-cmake-3.15-gcc-5-conan-1.19'
              args '--runtime=nvidia'
            }
          }
          steps {
            sh 'cd build-gcc-5 && make test'
          }
          post {
            always {
              step([
                $class: 'XUnitBuilder',
                thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
                tools: [[$class: 'GoogleTestType', pattern: 'build-gcc-5/Testing/*.xml']]
              ])
            }
          }
        }
        stage('gcc-8') {
          agent {
            docker {
              reuseNode true
              image 'braintwister/ubuntu-18.04-cuda-10.1-cmake-3.15-gcc-8-conan-1.19'
              args '--runtime=nvidia'
            }
          }
          steps {
            sh 'cd build-gcc-8 && make test'
          }
          post {
            always {
              step([
                $class: 'XUnitBuilder',
                thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
                tools: [[$class: 'GoogleTestType', pattern: 'build-gcc-8/Testing/*.xml']]
              ])
            }
          }
        }
        stage('clang-6') {
          agent {
            docker {
              reuseNode true
              image 'braintwister/ubuntu-18.04-cuda-10.1-cmake-3.15-clang-6-conan-1.19'
              args '--runtime=nvidia'
            }
          }
          steps {
            sh 'cd build-clang-6 && make test'
          }
          post {
            always {
              step([
                $class: 'XUnitBuilder',
                thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
                tools: [[$class: 'GoogleTestType', pattern: 'build-clang-6/Testing/*.xml']]
              ])
            }
          }
        }
      }
    }
    stage('Doxygen') {
      agent {
        docker {
          reuseNode true
          image 'braintwister/ubuntu-18.04-cuda-10.1-cmake-3.15-gcc-7-conan-1.19-doxygen-1.8.13'
          args '--runtime=nvidia'
        }
      }
      steps {
        sh './build.sh doc Release'
        publishHTML( target: [
          allowMissing: false,
          alwaysLinkToLastBuild: false,
          keepAll: true,
          reportName: 'Doxygen',
          reportDir: 'build-doc/doxygen/html',
          reportFiles: 'index.html'
        ])
      }
    }
    stage('Deploy') {
      agent {
        dockerfile {
          reuseNode true
          filename 'devel/Dockerfile-deploy'
          args '--runtime=nvidia'
        }
      }
      steps {
        sh '''
            export CONAN_USER_HOME=$PWD/conan-gcc-8
            cd build-gcc-8
            make package
        '''
      }
      post {
        success {
          archiveArtifacts artifacts: "build*/Pink*", fingerprint: true
        }
      }
    }
  }
  post {
    success {
      mail to: 'bernd.doser@h-its.org', subject: "SUCCESS: ${currentBuild.fullDisplayName}", body: "All fine."
    }
    failure {
      mail to: 'bernd.doser@h-its.org', subject: "FAILURE: ${currentBuild.fullDisplayName}", body: "Failed."
    }
  }
}
