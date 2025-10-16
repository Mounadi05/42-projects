setup:
	@chmod 600 ./ansible.pem
	@echo "Running Ansible playbook with AWS credentials..."
	@ansible-playbook setup.yaml -vvv

clean:
	@echo "Cleaning up resources..."
	@ansible-playbook clean.yaml
